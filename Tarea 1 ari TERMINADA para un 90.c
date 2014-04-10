#include <stdio.h>
#include <string.h>
#include <time.h>

enum tipo_post{
	graves,
	oldfag,
	newfag,
	gores,
	happy
	};

typedef struct tipos{
	int id_tipo;
	enum tipo_post type;
	int calidad;
	} tipo;

typedef struct admins{
	int id_admin;
	char nombre[30];
	unsigned int edad;
	} admin;

typedef struct posts{
	int id_post;
	int id_admin;
	int id_tipo;
	char fecha[30];
	char imagen[30];
	char leyenda[100];
	char descripcion[100];
	int likes;
	int dislikes;
	} post;

typedef struct comentarios{
	int id_comentario;
	int id_post;
	int id_usuario;
	int id_comentario_respuesta;
	char texto[100];
	int calificacion;
	} comentario;
        
typedef struct usuarios{
	int id_usuario;
	int id_usuario_sigue;
	char fecha_creacion[30];
	enum tipo_post preferencia;
	char avatar[30];
	char bio[100];
	int seguidores;
	} usuario;

void mostrar_usuarios(FILE *archivo){ //hacer que estos se ordenen a partir del que posee mas seguidores
	int id, tamanio_archivo;
	char pref[10];
	usuario u;
	fseek(archivo, 0, SEEK_END);
	tamanio_archivo=ftell(archivo);
	//Busqueda de usuario en todo el archivo
	for (id=sizeof(usuario); id<=(tamanio_archivo-sizeof(usuario)); id+=sizeof(usuario)){
		fseek(archivo, id, SEEK_SET);
		fread(&u, sizeof(usuario), 1, archivo);
		if (u.preferencia==graves) strcpy(pref,"Graves");
		else if (u.preferencia==oldfag) strcpy(pref,"Oldfag");
		else if (u.preferencia==newfag) strcpy(pref,"Newfag");
		else if (u.preferencia==gores) strcpy(pref,"Gores");
		else if (u.preferencia==happy) strcpy(pref,"Happy");
		printf("%s",u.avatar);
		if (strlen(u.avatar)<8) printf("\t");
		printf("\tPreferencia de posts: %s\n",pref);//borrar el id usuario
		printf("Seguidores: %d\n",u.seguidores);
	}
	if (tamanio_archivo==sizeof(usuario)) printf("Actualmente no existen usuarios\n");
	fclose(archivo);
}

int mostrar_post(FILE *archivo, enum tipo_post pref){ //archivo de solo lectura y la preferencia que se mostrara
	int post_mostrados=0, id, tamanio_archivo;
	post p;
	tipo t;
	
	fseek(archivo, 0, SEEK_END);
	tamanio_archivo=ftell(archivo);
	
	if (archivo==NULL || tamanio_archivo==0){
		printf("No existen post de esta categoria\n");
		return 0;
	}
	FILE *tip = fopen("archivo_tipo.dat","rb");
	if (tip==NULL){
		printf("error al abrir el archivo_tipo");
	}
	//Busqueda de posts en todo el archivo
	for (id=0; id<=(tamanio_archivo-sizeof(post)); id+=sizeof(post)){
		fseek(archivo, id, SEEK_SET);
		fread(&p, sizeof(post), 1, archivo);
		fseek(tip, p.id_tipo, SEEK_SET);
		fread(&t, sizeof(tipo), 1, tip);
		if (t.type==pref){
			if (post_mostrados>0) printf("\t");
			printf("post: \t\t%s\n\tfecha: \t\t%s\n\tleyenda:\t%s\n\n", p.imagen, p.fecha, p.leyenda);
			post_mostrados++;
		}
	}
	if (post_mostrados==0){
		printf("No existen post de esta categoria\n\n");
		return 0;
	}
	fclose(archivo);
	return post_mostrados;
}

int mostrar_comentarios(FILE *archivo, int id_post){ //archivo de solo lectura de comentarios y la id del post donde se encuentra
	int id, tamanio_archivo, comentarios_mostrados=0;
	comentario com;
	usuario u;
	
	fseek(archivo, 0, SEEK_END);
	tamanio_archivo=ftell(archivo);
	if (archivo==NULL || tamanio_archivo==0){
		printf("[No existen comentarios para este post]\n");
		return 0;
	}
	fseek(archivo, 0, SEEK_END);
	tamanio_archivo=ftell(archivo);
	FILE *usrs = fopen("archivo_usuario.dat","rb");
	//Busqueda de posts en todo el archivo
	for (id=0; id<=(tamanio_archivo-sizeof(comentario)); id+=sizeof(comentario)){
		fseek(archivo, id, SEEK_SET);
		fread(&com, sizeof(comentario), 1, archivo);
		fseek(usrs, com.id_usuario, SEEK_SET);
		fread(&u, sizeof(usuario), 1, usrs);
		if (com.id_post==id_post){
			if (com.id_usuario==0) {
				printf("%d) [usuario eliminado]: %s\n\n",comentarios_mostrados+1, com.texto);
				comentarios_mostrados++;
				continue;
			}
//			printf("leyendo usuario\nID segun comentario: %d\nID segun usuario: %d\navatar: %s\n",com.id_usuario, u.id_usuario, u.avatar);
			printf("%d) %s: %s\n\n",comentarios_mostrados+1, u.avatar, com.texto);
			comentarios_mostrados++;
		}
	}
	if (comentarios_mostrados==0){
		printf("[No existen comentarios para este post]\n\n");
		return 0;
	}
	fclose(archivo);
	return comentarios_mostrados;
}

int buscar_id_usuario(char nombre[30], FILE *archivo){
	int id, tamanio_archivo;
	usuario u;
	fseek(archivo, 0, SEEK_END);
	tamanio_archivo=ftell(archivo);
	//Busqueda de usuario en todo el archivo
	for (id=sizeof(usuario); id<=(tamanio_archivo-sizeof(usuario)); id+=sizeof(usuario)){
		fseek(archivo, id, SEEK_SET);
		fread(&u, sizeof(usuario), 1, archivo);
		if (strcmp(nombre,(u.avatar))==0){	//usuario encontrado
			return id;
		}
		else if (id==(tamanio_archivo-sizeof(usuario))){ //usuario no encontrado
			return -1;
		}
	}
	return -1; //algo fallo.. no deberia aparecer nunca
}

int buscar_id_post(char nombre[30], FILE *archivo){
	int id, tamanio_archivo;
	post p;
	fseek(archivo, 0, SEEK_END);
	tamanio_archivo=ftell(archivo);
	//Busqueda de post en todo el archivo
	for (id=0; id<=(tamanio_archivo-sizeof(post)); id+=sizeof(post)){
		fseek(archivo, id, SEEK_SET);
		fread(&p, sizeof(post), 1, archivo);
		if (strcmp(nombre, p.imagen)==0){	//usuario encontrado
			return id;
		}
		else if (id==(tamanio_archivo-sizeof(post))){ //usuario no encontrado
			return -1;
		}
	}
	return -1; //algo fallo.. no deberia aparecer nunca
}

int copiar_archivo(char *f_org,char *f_dest){
	FILE *fp_org = fopen(f_org,"rb"),*fp_dest = fopen(f_dest,"wb"); 
	char c;
	int tamanio_origen, tamanio_destino;
	
	if(fp_org==NULL || fp_dest==NULL){ 		//error al leer o escribir
		printf("error al leer o escribir durante la copia (copiar_archivo)\n");
		return 1; 
	}
	
	c=getc(fp_org);
	while(feof(fp_org)==0){
		putc(c,fp_dest);
        c=getc(fp_org);
		}
	
	fseek(fp_org, 0, SEEK_END);
	fseek(fp_dest, 0, SEEK_END);
	tamanio_origen=ftell(fp_org);
	tamanio_destino=ftell(fp_dest);
	if(tamanio_origen!=tamanio_destino){ 	//si los tamaños de los archivos son distintos
//		printf("error: archivos tienen distintos tamanios:\norigen: %d\ndestino: %d\n",tamanio_origen,tamanio_destino);
		return 1; 
	}

//	printf("se crea exitosamente el archivo temp de usuarios\n");
	fclose(fp_org);
	fclose(fp_dest);
	return 0;
}

int borrar_comentarios_de_usuario(int id_usuario_borrado, int usuario_movido){ //toma el ultimo registro y lo coloca en la posicion id reemplazando al que estaba en esa posicion
	FILE *fp_org_com = fopen("archivo_comentario.dat","rb"),*fp_dest_com = fopen("archivo_comentario_temp.dat","wb"); 
	FILE *fp_org_usr = fopen("archivo_usuario.dat","rb");

	comentario com;
	usuario u;
	
	int i, tamanio_origen, tamanio_destino;

	if(fp_org_com==NULL || fp_dest_com==NULL || fp_org_usr==NULL/* || fp_dest_usr==NULL*/){
		printf("error al leer o escribir durante el cambio comentario del usuario\n");
		return 1; //error al leer o escribir
	}
	//printf("\nRecorrido de los comentarios del usuario:\n");
	fseek(fp_org_com, 0, SEEK_END);
	tamanio_origen=ftell(fp_org_com);
	if (tamanio_origen==0){
		//no existen comentarios, por lo que no es nesesario borrarles nada
		return 0;
	}
	//recorriendo los comentarios
	for (i=0; i<=(tamanio_origen-sizeof(comentario)); i+=sizeof(comentario)){
		fseek(fp_org_com, i, SEEK_SET);
		fread(&com, sizeof(comentario), 1, fp_org_com);
		fseek(fp_org_usr, com.id_usuario, SEEK_SET);
		fread(&u, sizeof(comentario), 1, fp_org_usr);
//		printf("leyendo comentario %s (ID comentario: %d)\n\tcon id usuario %d\n", com.texto, com.id_comentario, com.id_usuario);
//		printf("segun el archivo_usuario, el usuario que comento (con id %d) es %s\n\n", u.id_usuario, u.avatar);
		if (com.id_usuario==id_usuario_borrado){
			com.id_usuario=0;
			printf("***cambia la ID_usuario (del comentario) %d a: 0\n",id_usuario_borrado);
		}
		if (com.id_usuario==usuario_movido){
			com.id_usuario=id_usuario_borrado;
			printf("***cambia la ID_usuario (del comentario) %d a: %d\n", com.id_usuario, id_usuario_borrado);
		}
		fseek(fp_dest_com, i, SEEK_SET);
		fwrite(&com, sizeof(comentario), 1, fp_dest_com);
	}
	//finaliza etapa de copiado
	fseek(fp_dest_com, 0, SEEK_END);
	tamanio_destino=ftell(fp_dest_com);
	if(tamanio_origen!=tamanio_destino){ //si el tamaño de inicio es distinto al de destino incluyendo el tamaño que ocuparia otro usuario dentro de el
		printf("el nuevo archivo_comentario no se copio hasta donde deberia\n");
		return 1;	//problema al copiar
	}
	else{ //el archivo temporal es del mismo tamaño que el anterior
		remove("archivo_comentario.dat");
		rename("archivo_comentario_temp.dat","archivo_comentario.dat");
	}
	fclose(fp_org_com);
	fclose(fp_dest_com);
	return 0;	//copia exitosa
}

int borrar_usuario_temp(char *f_org,char *f_dest, int id){ //toma el ultimo registro y lo coloca en la posicion id reemplazando al que estaba en esa posicion
	FILE *fp_org = fopen("archivo_usuario.dat","rb"),*fp_dest = fopen("archivo_usuario_temp.dat","wb"); 
	usuario u;
	int i, tamanio_origen, tamanio_destino, tamanio_usuario=sizeof(usuario);

	if(fp_org==NULL || fp_dest==NULL){
		printf("error al leer o escribir durante la copia\n");
		return 1; //error al leer o escribir
	}
	fseek(fp_org, 0, SEEK_END);
	tamanio_origen=ftell(fp_org);
	
	if (borrar_comentarios_de_usuario(id, tamanio_origen-sizeof(usuario))==1){
		printf("error al cambiar el nombre de los comentarios del usuario");
		return 1;
	}
	//comenzar a copiar todos los parametros antes del ultimo
	for (i=0; i<(tamanio_origen-sizeof(usuario)); i+=sizeof(usuario)){
		fseek(fp_org, i, SEEK_SET);
		if (i==id){
			fseek(fp_org, -tamanio_usuario, SEEK_END); //se posiciona en el principio del ultimo registro de origen
			fread(&u, sizeof(usuario), 1, fp_org);
			u.id_usuario=id;
		}
		else {
			fread(&u, sizeof(usuario), 1, fp_org);
		}
		fseek(fp_dest, i, SEEK_SET);
		fwrite(&u, sizeof(usuario), 1, fp_dest);
	}
	//finaliza etapa de copiado
	fseek(fp_dest, 0, SEEK_END);
	tamanio_destino=ftell(fp_dest);
	if(tamanio_origen!=(tamanio_destino+sizeof(usuario))){ //si el tamaño de inicio es distinto al de destino incluyendo el tamaño que ocuparia otro usuario dentro de el
		printf("el nuevo semi archivo_usuario no se copio hasta donde deberia\n");
		return 1;	//problema al copiar
	}
	fclose(fp_org);
	fclose(fp_dest);
	return 0;	//copia exitosa
	
}

int borrar_post_temp(char *f_org,char *f_dest, int id){ //toma el ultimo registro y lo coloca en la posicion id reemplazando al que estaba en esa posicion
	FILE *fp_org = fopen(f_org,"rb"),*fp_dest = fopen(f_dest,"wb");

	post p;
	int i, tamanio_origen, tamanio_destino, tamanio_post=sizeof(post);
	
	if(fp_org==NULL || fp_dest==NULL){
		printf("error al leer o escribir durante la copia (borrar_post_temp)\n");
		return 1; //error al leer o escribir
	}
	fseek(fp_org, 0, SEEK_END);
	tamanio_origen=ftell(fp_org);
	//comenzar a copiar todos los parametros antes del ultimo
	for (i=0; i<(tamanio_origen-sizeof(post)); i+=sizeof(post)){
		fseek(fp_org, i, SEEK_SET);
		if (i==id){
			fseek(fp_org, -tamanio_post, SEEK_END); //se posiciona en el principio del ultimo registro de origen
		}
		fread(&p, sizeof(post), 1, fp_org);
		fseek(fp_dest, i, SEEK_SET);
		fwrite(&p, sizeof(post), 1, fp_dest);
	}
	//finaliza etapa de copiado
	fseek(fp_dest, 0, SEEK_END);
	tamanio_destino=ftell(fp_dest);
	if(tamanio_origen!=(tamanio_destino+sizeof(post))){ //si el tamaño de inicio es distinto al de destino incluyendo el tamaño que ocuparia otro post dentro de el
		printf("el nuevo semi archivo_post no se copio hasta donde deberia\n");
		return 1;	//problema al copiar
	}
	fclose(fp_org);
	fclose(fp_dest);
	return 0;	//copia exitosa
}

void log_usuario(int id_usuario){
	char c[10], name[30];
	int intc=0, posts_vistos=0;
	usuario u, u_original;
	post p_original;		//para leer posts
	admin a_original;	//para leer al admin creador de cada post
	comentario com/*, com_original*/;	//para leer y crear un nuevo comentario
	
	menu_user:{ // mismo formato que el menu principal
		FILE *usrs = fopen("archivo_usuario.dat","rb");
		fseek(usrs, id_usuario, SEEK_SET);
		fread(&u_original, sizeof(usuario),1, usrs);
		printf("\n-------------------------------------------------\nPosts actuales:\n\n");
		if (u_original.preferencia==graves)	     printf("Graves: ");
		else if (u_original.preferencia==oldfag) printf("Oldfag: ");
		else if (u_original.preferencia==newfag) printf("Newfag: ");
		else if (u_original.preferencia==gores) printf("Gores:  ");
		else if (u_original.preferencia==happy) printf("Happy:  ");
		posts_vistos=mostrar_post(fopen("archivo_post.dat","rb"),u_original.preferencia);
		printf("-------------------------------------------------\n");
		printf("Usuario:\t%s\n",u_original.avatar);
		printf("-------------------------------------------------\n");
		printf("Menu usuario:\n\t1.- Editar Perfil\n\t2.- Seguir a un usuario\n\t3.- Ver un post\n\t4.- Volver al menu principal.\nSeleccionar: ");
		gets(c);
		if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0 && strcmp(c,"4")!=0){
			printf(" Respuesta no valida\n\n");
			goto menu_user;
		}
		if (strcmp(c,"1")==0){intc=1;}
		else if(strcmp(c,"2")==0){intc=2;}
		else if(strcmp(c,"3")==0){intc=3;}
		else if(strcmp(c,"4")==0){intc=4;}
	}

	FILE *usrs = fopen("archivo_usuario.dat","rb");
	FILE *usrstmp = fopen("archivo_usuario_temp.dat","wb");
	FILE *pos = fopen("archivo_post.dat","rb");
	FILE *postmp = fopen("archivo_post_temp.dat","wb");
	FILE *admn = fopen("archivo_admin.dat","rb");
	FILE *comen = fopen("archivo_comentario.dat","rb");
	FILE *comentmp = fopen("archivo_comentario_temp.dat","wb");

	switch (intc){
		case 1: modificar_usuario:{				//1.- editar perfil
			printf("\nMenu de edicion:\n\t1.- Editar preferencia\n\t2.- Editar biografia\n\t3.- Volver.\nSeleccionar: ");
			gets(c);
			if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0){
				printf(" Respuesta no valida\n\n");
				goto modificar_usuario;
			}
			if (strcmp(c,"1")==0){intc=1;}
			else if(strcmp(c,"2")==0){intc=2;}
			else if(strcmp(c,"3")==0){intc=3;}
			
			while(copiar_archivo("archivo_usuario.dat","archivo_usuario_temp.dat")){	//crear temp
				printf("error al crear el archivo temporal de usuarios\n");
			}
			
			switch (intc){
				case 1:	menu_preferencia_usuario:{ //nueva preferencia
						printf("Nueva preferencia:\n 1.- Graves\n 2.- Oldfags\n 3.- Newfags\n 4.- Gores\n 5.- Happy\n Seleccionar: ");
						gets(c);
						if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0 && strcmp(c,"4")!=0 && strcmp(c,"5")!=0){
						printf(" Respuesta no valida\n\n");
						goto menu_preferencia_usuario;
						}

					if (strcmp(c,"1")==0){intc=1;}					// no deberia ser un else if igual? total el anterior es un if...
					else if(strcmp(c,"2")==0){intc=2;}	
					else if(strcmp(c,"3")==0){intc=3;}
					else if(strcmp(c,"4")==0){intc=4;}
					else if(strcmp(c,"5")==0){intc=5;}
					
					switch (intc){ 		// set nueva preferencia
					case 1: u.preferencia = graves;
					break;
					case 2: u.preferencia = oldfag;
					break;
					case 3: u.preferencia = newfag;
					break;
					case 4: u.preferencia = gores;
					break;
					case 5: u.preferencia = happy;
					break;
					}
					fseek(usrs, id_usuario, SEEK_SET);								//ir al lugar del usuario
					fread(&u_original, sizeof(usuario),1, usrs);					// AHAHA
					u.id_usuario=id_usuario;										
					u.id_usuario_sigue=u_original.id_usuario_sigue;					//
					strcpy(u.fecha_creacion,u_original.fecha_creacion);						//pasar datos del original al u
					strcpy(u.avatar,u_original.avatar);								//
					strcpy(u.bio, u_original.bio);									//
					fseek(usrstmp, id_usuario, SEEK_SET);
					if (fwrite(&u, sizeof(usuario), 1, usrstmp)==1){				//escritura correcta
						remove("archivo_usuario.dat");								//borrar original
						rename("archivo_usuario_temp.dat","archivo_usuario.dat"); 	//temp ahora es el original
						printf("-----------------------------------------\n");
						printf("Edicion de preferencia exitosa\n");
						printf("-----------------------------------------\n");
						fclose(usrstmp);
						goto menu_user;
					}
					break;
				
				}
				case 2:	{	//nueva biografia
					fseek(usrs, id_usuario, SEEK_SET);								//ir al lugar del usuario
					fread(&u_original, sizeof(usuario),1, usrs);
					printf("Biografia actual:\n\t%s\n-------------------------------------------------\n",u_original.bio);				// (!) que imprima la bio anterior, no se si es correcto...
					
					menu_nueva_biografia:{
						printf("ingresar biografia nueva?\n\t1.- Si\n\t2.- Cancelar\nSeleccion: ");
						gets(c);
						
						if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0){
							printf(" Respuesta no valida\n\n");
							goto menu_nueva_biografia;
						}}
					if (strcmp(c,"1")==0){		// ingresar nueva
						printf("-------------------------------------------------\ningrese su nueva biografia:\n");
						gets(u.bio);
						printf("\n-------------------------------------------------\n");
						
						while(copiar_archivo("archivo_usuario.dat","archivo_usuario_temp.dat")){	//crear temp
							printf("error al crear el archivo temporal de usuarios\n");
						}
						fseek(usrs, id_usuario, SEEK_SET);								//ir al lugar del usuario
						fread(&u_original, sizeof(usuario),1, usrs);					// AHAHA
						u.id_usuario=id_usuario;										
						u.id_usuario_sigue=u_original.id_usuario_sigue;					//
						strcpy(u.fecha_creacion,u_original.fecha_creacion);				//pasar datos del original al u
						strcpy(u.avatar,u_original.avatar);								//
						u.preferencia=u_original.preferencia;							//(!) revisar si es correcto
						fseek(usrstmp, id_usuario, SEEK_SET);
						if (fwrite(&u, sizeof(usuario), 1, usrstmp)==1){				//escritura correct
							remove("archivo_usuario.dat");								//borrar original
							rename("archivo_usuario_temp.dat","archivo_usuario.dat"); 	//temp ahora es el original
							printf("-----------------------------------------\n");
							printf("Edicion de biografia exitosa\n-----------------------------------------\n");//verificar despues el renombramiento, correcto ==0
							fclose(usrstmp);
							goto menu_user;
						}
						printf("\nno se guardo biografia D:\n");
					}
					else if(strcmp(c,"2")==0) goto modificar_usuario;
					break;
				}
				case 3:	goto menu_user;
						break;
			}
		}
		case 2:{								//2.- seguir a un usuario
			fseek(usrs, id_usuario, SEEK_SET);
			fread(&u_original, sizeof(usuario),1, usrs);
			fseek(usrs, u_original.id_usuario_sigue, SEEK_SET);
			fread(&u_original, sizeof(usuario),1, usrs);//usuario que se esta siguiendo
			printf("\n-------------------------------------------------\nUsuarios disponibles:\n");
			mostrar_usuarios(fopen("archivo_usuario.dat","rb"));
			printf("Actualmente sigues a: %s\n",u_original.avatar);
			printf("\n-----------------------------------------\n");
			ingresar_usuario_a_seguir:{
				printf("Ingrese nombre de usuario a seguir: ");
				gets(name);
				if (strcmp(name,"0")==0){
					goto menu_user;
				}
				int id=buscar_id_usuario(name, usrs);
				if (id==-1){
					printf("no se encuentra el nombre, intentelo nuevamente.\n\tingrese '0' para cancelar\n");
					goto ingresar_usuario_a_seguir;
				}
			}
			if(copiar_archivo("archivo_usuario.dat","archivo_usuario_temp.dat")){	//crear temp
				printf("error al crear el archivo temporal de usuarios\n");
				goto menu_user;
			}
			int id=buscar_id_usuario(name, usrs);
			fseek(usrs, id_usuario, SEEK_SET);								//ir al lugar del usuario actual
			fread(&u, sizeof(usuario),1, usrs);
			
			if (u.id_usuario_sigue==id){									//si ya seguias al usuario escrito
				printf("Ya seguias a este usuario\n");
				goto ingresar_usuario_a_seguir;
			}
			else if (u.id_usuario==id){
				printf("No puedes seguirte a ti mismo! eso es patetico D:\n");
				goto ingresar_usuario_a_seguir;
			}
			else if (u.id_usuario_sigue!=0){ 									//si antes ya seguia a alguien
				fseek(usrs, u.id_usuario_sigue, SEEK_SET); 					//ir al usuario que se seguia
				fread(&u_original, sizeof(usuario),1, usrs);				//u_original tiene los valores del usuario al que seguia
				u_original.seguidores--;									//se resta uno a sus seguidores
				fseek(usrstmp, u.id_usuario_sigue, SEEK_SET);
				if (fwrite(&u_original, sizeof(usuario), 1, usrstmp)!=1){	//se sobreescribe en el archivo temporal los datos de quien seguia
					printf("Error de escritura\n");
					goto menu_user;
				}
			}
			u.id_usuario_sigue=id;
			fseek(usrstmp, id_usuario, SEEK_SET);							//ir al usuario actual (en el archivo temp)
			if (fwrite(&u, sizeof(usuario), 1, usrstmp)==1){				//se sobreescribe en el archivo temporal los datos el usuario actual.. usuario

				fseek(usrs, id, SEEK_SET);									//ir al usuario a seguir
				fread(&u_original, sizeof(usuario),1, usrs);				//u_original tiene los valores del usuario nuevo al que sigue
				u_original.seguidores++;
				fseek(usrstmp, id, SEEK_SET);								//ir al usuario a seguir en el archivo temp
				if (fwrite(&u_original, sizeof(usuario), 1, usrstmp)==1){
					remove("archivo_usuario.dat");								//borrar original
					rename("archivo_usuario_temp.dat","archivo_usuario.dat"); 	//temp ahora es el original
					printf("-----------------------------------------\n");
					printf("#follow registrado\n");
					printf("-----------------------------------------\n");
					fclose(usrstmp);
					goto menu_user;
				}
			}
			printf("Error de escritura\n");
			}break;
		case 3: seleccionar_post:{				//3.- ver un post
				if (pos==NULL || posts_vistos==0){
					printf("Actualmente no existen post");
					goto menu_user;
				}
				printf("Escriba el titulo del post: ");
				gets(name);
				if (strcmp(name,"0")==0){
					goto menu_user;
				}
				int id=buscar_id_post(name, pos);
				if (id==-1){
					printf("el post no existe\n   escriba 0 para volver al menu anterior\n\n");
					goto seleccionar_post;
				}
				menu_post:{
				pos = fopen("archivo_post.dat","rb");
				admn = fopen("archivo_admin.dat","rb");
				postmp = fopen("archivo_post_temp.dat","wb");
				comen = fopen("archivo_comentario.dat","rb");
				comentmp = fopen("archivo_comentario_temp.dat","wb");
				//se recolectan e imprimen los datos del post
				fseek(pos, id, SEEK_SET);
				fread(&p_original, sizeof(post), 1, pos);
				fseek(admn, p_original.id_admin, SEEK_SET);
				fread(&a_original, sizeof(admin), 1, admn);
				printf("\n-----------------------------------------\n");
				printf("Creador del post:\t%s\nFecha de creacion:\t%s\n\n",a_original.nombre, p_original.fecha);
				printf("Post:\t\t%s\n",p_original.imagen);
				printf("Leyenda:\t%s\nDescripcion:\t%s\n",p_original.leyenda, p_original.descripcion);
				printf("\n\tlikes: %d  - dislikes: %d\n\n",p_original.likes, p_original.dislikes);
				mostrar_comentarios(comen,id);//comentarios mediante el archivo y la id del post
				printf("-----------------------------------------\n\n");
				
				//preguntar si se desea dar like, dislike o comentar
				printf("Menu post:\n\t1.- Dar like\n\t2.- Dar dislike\n\t3.- Comentar.\n\t4.- Volver al menu usuario.\nSeleccionar: ");
				gets(c);
				if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0 && strcmp(c,"4")!=0){
					printf("respuesta no valida\n\n");
					goto menu_post;
				}
				if (strcmp(c,"1")==0){intc=1;} //probar intentando cambiar la variable
				else if(strcmp(c,"2")==0){intc=2;}
				else if(strcmp(c,"3")==0){intc=3;}
				else if(strcmp(c,"4")==0){intc=4;}
				}
				
				switch(intc){
					case 1: {//dar like... se pueden dar ilimitados
							//ya se recolectaron los datos antes en p_original
							while(copiar_archivo("archivo_post.dat","archivo_post_temp.dat")){
								printf("error al crear el archivo temporal de usuarios\n");
							}
							p_original.likes++;
							fseek(postmp, id, SEEK_SET);
							if (fwrite(&p_original, sizeof(post), 1, postmp)!=1){
								printf("error al escribir sobre el archivo temporal\n");
								goto menu_post;
							}
							else{
								remove("archivo_post.dat");
								rename("archivo_post_temp.dat","archivo_post.dat"); //condicionar como se debe
								printf("-----------------------------------------\n");
								printf("Has dado like al post (Y)\n");
								printf("-----------------------------------------\n");
								fclose(postmp);
								goto menu_post;
							}
					}
					case 2: {//dar dislike... se pueden dar ilimitados
							//ya se recolectaron los datos antes en p_original
							while(copiar_archivo("archivo_post.dat","archivo_post_temp.dat")){
								printf("error al crear el archivo temporal de usuarios\n");
							}
							p_original.dislikes++;
							fseek(postmp, id, SEEK_SET);
							if (fwrite(&p_original, sizeof(post), 1, postmp)!=1){
								printf("error al escribir sobre el archivo temporal\n");
								goto menu_post;
							}
							else{
								remove("archivo_post.dat");
								rename("archivo_post_temp.dat","archivo_post.dat"); //condicionar como se debe
								printf("-----------------------------------------\n");
								printf("Has dado dislike al post (N)\n");
								printf("-----------------------------------------\n");
								fclose(postmp);
								goto menu_post;
							}
					}
					case 3: {//comentar
							if (comen==NULL){
								comen = fopen("archivo_comentario.dat","wb");
							}
							copiar_archivo("archivo_comentario.dat","archivo_comentario_temp.dat");
							//obtencion de datos del comentario
							fseek(comentmp, 0, SEEK_END);
							com.id_comentario=ftell(comentmp);
							com.id_post=id;		//se pide en seleccionar post.. linea 481
							com.id_usuario=id_usuario;	//se pide en los parametros
							com.id_comentario_respuesta=0;
							printf("Escriba su comentario:\n");
							gets(com.texto);
							com.calificacion=5; //punto_intermedio
							if (fwrite(&com, sizeof(comentario),1, comentmp)==1){ //usar un while en vez del if
								remove("archivo_comentario.dat");
								if(rename("archivo_comentario_temp.dat","archivo_comentario.dat")==0){
									printf("-----------------------------------------\n");
									printf("Creacion exitosa del comentario\n");
									printf("-----------------------------------------\n");
								}
								else{
									printf("Problema al renombrar el archivo comentario\n");
								}
							}
							else{
								printf("Problema al escribir en el archivo temporal\n");
							}
							fclose(comentmp);
							goto menu_post;

					}
					case 4: goto menu_user;
				}
		}//termino del caso 3 en el switch anterior (seleccionar post)
		case 4: break;							//4.- volver
	}
	fclose(usrs);
	fclose(usrstmp);
	fclose(pos);
	fclose(postmp);
	fclose(admn);
	fclose(comen);
	fclose(comentmp);
	remove("archivo_usuario_temp.dat");
	remove("archivo_post_temp.dat");
	remove("archivo_comentario_temp.dat");
}



void log_administrador(int id_admin){
	char c[10], name[30], desc[100];
	int intc=0;
	usuario u, u_original;
	post p, p_original;
	tipo t;
	time_t strtime;
	struct tm * timeinfo;
	
	//hacer vista de top 5 de post
	//hacer vista de usuarios existentes (por cualquier admin)------ FALTA ORDENARLOS

	menu_admin:{
	printf("\n-----------------------------------------\nUsuarios disponibles:\n");
	mostrar_usuarios(fopen("archivo_usuario.dat","rb"));
	printf("\n-----------------------------------------\nPost disponibles (todas las categorias):\n");
	printf("Graves: ");
	mostrar_post(fopen("archivo_post.dat","rb"),graves);
	printf("Oldfag: ");
	mostrar_post(fopen("archivo_post.dat","rb"),oldfag);
	printf("Newfag: ");
	mostrar_post(fopen("archivo_post.dat","rb"),newfag);
	printf("Gores:  ");
	mostrar_post(fopen("archivo_post.dat","rb"),gores);
	printf("Happy:  ");
	mostrar_post(fopen("archivo_post.dat","rb"),happy);
	printf("-----------------------------------------\n");
	printf("Menu administrador:\n\t1.- Crear nuevo usuario\n\t2.- Modificar usuario\n\t3.- Eliminar usuario\n\t4.- Crear nuevo post\n\t5.- Modificar post\n\t6.- Eliminar post.\n\t7.- Volver al menu principal.\nSeleccionar: ");
	gets(c);
	if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0 && strcmp(c,"4")!=0 && strcmp(c,"5")!=0 && strcmp(c,"6")!=0 && strcmp(c,"7")!=0){
		printf(" Respuesta no valida\n\n");
		goto menu_admin;
	}
	if (strcmp(c,"1")==0){intc=1;}
	else if(strcmp(c,"2")==0){intc=2;}
	else if(strcmp(c,"3")==0){intc=3;}
	else if(strcmp(c,"4")==0){intc=4;}
	else if(strcmp(c,"5")==0){intc=5;}
	else if(strcmp(c,"6")==0){intc=6;}
	else if(strcmp(c,"7")==0){intc=7;}
	}
	
	FILE *usrs = fopen("archivo_usuario.dat","rb"); //ya esta llamado anteriormente
	FILE *usrstmp = fopen("archivo_usuario_temp.dat","wb");
	FILE *pos = fopen("archivo_post.dat","rb");
	FILE *postmp = fopen("archivo_post_temp.dat","wb");
	FILE *tip = fopen("archivo_tipo.dat","rb");
	FILE *tiptmp = fopen("archivo_tipo_temp.dat","wb");

	switch (intc){
		case 1:	{					//crear usuario
				while(copiar_archivo("archivo_usuario.dat","archivo_usuario_temp.dat")){
					printf("error al crear el archivo temporal\n");
				}
				//obteniendo datos
				fseek(usrstmp, 0, SEEK_END);
				u.id_usuario=ftell(usrstmp);//la nueva id esta al final del archivo anterior
				u.id_usuario_sigue=0;
				strtime=time(NULL); //fecha y hora actual
				timeinfo = localtime(&strtime);
				strftime(u.fecha_creacion, 30, "%d/%m/%y %I:%M%p", timeinfo);
				while(1){ //nombre de usuario en uso
					printf("Ingrese nombre de usuario a crear: ");
					gets(u.avatar);
					if (buscar_id_usuario(u.avatar, usrs)==-1){ //no se encuantra el nombre en uso
						if (strcmp(u.avatar,"0")==0 || strcmp(u.avatar,"")==0){
							continue;
						}
						break;
					}
					printf("   El nombre de usuario ya esta en uso, intente utilizar otro\n");
				}
				menu_preferencia:{
				printf(" Preferencia de posts:\n   1.- Graves\n   2.- Oldfags\n   3.- Newfags\n   4.- Gores\n   5.- Happy\n Seleccionar: ");
				gets(c);
				if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0 && strcmp(c,"4")!=0 && strcmp(c,"5")!=0){
					printf(" Respuesta no valida\n\n");
					goto menu_preferencia;
				}
				if (strcmp(c,"1")==0){intc=1;}
				else if(strcmp(c,"2")==0){intc=2;}
				else if(strcmp(c,"3")==0){intc=3;}
				else if(strcmp(c,"4")==0){intc=4;}
				else if(strcmp(c,"5")==0){intc=5;}
				
				switch (intc){ //preferencia
					case 1:	u.preferencia = graves;
							break;
					case 2:	u.preferencia = oldfag;
							break;
					case 3:	u.preferencia = newfag;
							break;
					case 4:	u.preferencia = gores;
							break;
					case 5:	u.preferencia = happy;
							break;
				}
				}
				strcpy(u.bio,"agrega una descripcion");
				u.seguidores = 0;
				if (fwrite(&u, sizeof(usuario), 1, usrstmp)==1){
					remove("archivo_usuario.dat");
					if(rename("archivo_usuario_temp.dat","archivo_usuario.dat")==0){
						printf("-----------------------------------------\n");
						printf("Creacion exitosa de usuario\n");
						printf("-----------------------------------------\n");
					}
					else{
						printf("Problema al renombrar el archivo temporal\n");
					}
					fclose(usrstmp);
				}
				goto menu_admin;
				}
		case 2:	modificar_usuario:{ //se pregunta lo que desea modificar... modifica el nombre de usuario y su preferencia
				printf("Ingrese nombre de usuario a modificar: ");
				gets(name);
				if (strcmp(name,"0")==0){
					goto menu_admin;
				}
				int id=buscar_id_usuario(name, usrs);
				if (id==-1){
					printf("el usuario no existe\n   escriba 0 para volver al menu anterior\n\n");
					goto modificar_usuario;
				}
				while(copiar_archivo("archivo_usuario.dat","archivo_usuario_temp.dat")){
					printf("error al crear el archivo temporal de usuarios\n");
				}
				
				fseek(usrs, id, SEEK_SET);
				fread(&u_original, sizeof(usuario),1, usrs);
				//recopilando nuevos datos
				u.id_usuario=id;
				u.id_usuario_sigue=u_original.id_usuario_sigue;
				strcpy(u.fecha_creacion,u_original.fecha_creacion);
//				printf("\nNombre de usuario actual: %s", u_original.avatar);	//pedir opinion a los demas sobre agregarlo... a mi no me convense
				printf("Ingrese nuevo nombre de usuario: ");
				gets(u.avatar);
				new_menu_preferencia:{
				printf("Nueva preferencia de posts:\n   1.- Graves\n   2.- Oldfags\n   3.- Newfags\n   4.- Gores\n   5.- Happy\n Seleccionar: ");
				gets(c);
				if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0 && strcmp(c,"4")!=0 && strcmp(c,"5")!=0){
					printf(" Respuesta no valida\n\n");
					goto new_menu_preferencia;
				}
				if (strcmp(c,"1")==0){intc=1;}
				else if(strcmp(c,"2")==0){intc=2;}
				else if(strcmp(c,"3")==0){intc=3;}
				else if(strcmp(c,"4")==0){intc=4;}
				else if(strcmp(c,"5")==0){intc=5;}
				
				switch (intc){ //preferencia
					case 1:	u.preferencia = graves;
							break;
					case 2:	u.preferencia = oldfag;
							break;
					case 3:	u.preferencia = newfag;
							break;
					case 4:	u.preferencia = gores;
							break;
					case 5:	u.preferencia = happy;
							break;
				}
				}
				strcpy(u.bio,u_original.bio);
				u.seguidores=u_original.seguidores;
				fseek(usrstmp, id, SEEK_SET);
				if (fwrite(&u, sizeof(usuario),1, usrstmp)==1){
					remove("archivo_usuario.dat");
					if(rename("archivo_usuario_temp.dat","archivo_usuario.dat")==0){
						printf("-----------------------------------------\n");
						printf("Edicion exitosa de usuario\n");
						printf("-----------------------------------------\n");
					}
					else{
						printf("Problema al renombrar el archivo temporal\n");
					}
					fclose(usrstmp);
				}
				else printf("Error de escritura, la modificacion no se llevo a cabo\n");
				goto menu_admin;
				}
		case 3:	borrar_usuario:{ 	//eliminar usuario
				printf("Ingrese nombre de usuario a borrar: ");
				gets(name);
				if (!strcmp("0",name)) goto menu_admin;
				int id=buscar_id_usuario(name, usrs); //donde se reemplazara el ultimo usuario
				if (id==-1){
					printf("usuario no encontrado\n");
					goto borrar_usuario;
				}
				
				//lo sig hace que los otros usuarios dejen de seguirlo
				if(copiar_archivo("archivo_usuario.dat","archivo_usuario_temp.dat")){	//crear temp
					printf("error al crear el archivo temporal de usuarios\n");
					goto menu_admin;
				}
				if(copiar_archivo("archivo_usuario.dat","archivo_usuario_respaldo.dat")){	//crear respaldo para no perder los seguidores del usuario si este no se elimina
					printf("error al crear el archivo de respaldo de usuarios\n");
					goto menu_admin;
				}
				fseek(usrs, 0, SEEK_END);
				int i, tamanio_origen=ftell(usrs);
				for (i=0; i<=(tamanio_origen-sizeof(usuario)); i+=sizeof(usuario)){	//encuentra a todos los usuarios
					fseek(usrs, i, SEEK_SET);
					fread(&u, sizeof(usuario), 1, usrs);
					if (u.id_usuario_sigue==id){
						u.id_usuario_sigue=0;
					}
					fseek(usrstmp, i, SEEK_SET);
					if (fwrite(&u, sizeof(usuario), 1, usrstmp)!=1){		//sobreescribe en el archivo temporal
						printf("Error en la escritura al archivo temporal\n");
						goto menu_admin;
					}
				}
				remove("archivo_usuario.dat");											//borrar original
				rename("archivo_usuario_temp.dat","archivo_usuario.dat"); 				//ya no posee los seguidores
				if (borrar_usuario_temp("archivo_usuario.dat","archivo_usuario_temp.dat",id)==0){
					remove("archivo_usuario.dat");
					remove("archivo_usuario_respaldo.dat");
					if(rename("archivo_usuario_temp.dat","archivo_usuario.dat")==0){
						printf("-----------------------------------------\n");
						printf("Eliminacion exitosa de usuario\n");
						printf("-----------------------------------------\n");
					}
					else{
						printf("Problema al renombrar el archivo temporal\n");
						remove("archivo_usuario.dat");									//borrar original
						rename("archivo_usuario_respaldo.dat","archivo_usuario.dat"); 	//temp ahora es el original
					}
				}
				else{
					printf("Error de escritura, la eliminacion no se llevo a cabo\n");
					remove("archivo_usuario.dat");									//borrar original
					rename("archivo_usuario_respaldo.dat","archivo_usuario.dat"); 	//temp ahora es el original
				}
				goto menu_admin;
				}
		case 4: {					//crear post
				if (pos==NULL){
					printf("actualmente no existen post\n");
					pos = fopen("archivo_post.dat","wb");
				}
				copiar_archivo("archivo_post.dat","archivo_post_temp.dat");
				//obtencion de datos del post
				fseek(postmp, 0, SEEK_END);
				p.id_post=ftell(postmp);
				p.id_admin=id_admin;	//se pide en los parametros
				if (tip==NULL){ //si no existe archivo_tipo.dat
					printf("actualmente no existen tipos de post\n");
					tip = fopen("archivo_tipo.dat","wb");
				}
				copiar_archivo("archivo_tipo.dat","archivo_tipo_temp.dat");
				fseek(tiptmp, 0, SEEK_END);
				t.id_tipo=ftell(tiptmp);
				//datos archivo tipo
				menu_tipo_post:{ //peticion del tipo de post
				printf("\n-----------------------------------------\n");
				printf("Tipo de post:\n   1.- Graves\n   2.- Oldfags\n   3.- Newfags\n   4.- Gores\n   5.- Happy\n Seleccionar: ");
				gets(c);
				if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0 && strcmp(c,"4")!=0 && strcmp(c,"5")!=0){
					printf(" Respuesta no valida\n\n");
					goto menu_tipo_post;
				}
				if (strcmp(c,"1")==0){intc=1;}
				else if(strcmp(c,"2")==0){intc=2;}
				else if(strcmp(c,"3")==0){intc=3;}
				else if(strcmp(c,"4")==0){intc=4;}
				else if(strcmp(c,"5")==0){intc=5;}
				switch (intc){ //preferencia
					case 1:	t.type = graves;
							break;
					case 2:	t.type = oldfag;
							break;
					case 3:	t.type = newfag;
							break;
					case 4:	t.type = gores;
							break;
					case 5:	t.type = happy;
							break;
				}
				}
				menu_calidad_post:{ //peticion de la calidad del tipo
				printf("\nCalidad del tipo del post? (1-10): ");
				gets(c);
				if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0 && strcmp(c,"4")!=0 && strcmp(c,"5")!=0 && strcmp(c,"6")!=0 && strcmp(c,"7")!=0 && strcmp(c,"8")!=0 && strcmp(c,"9")!=0 && strcmp(c,"10")!=0){
					printf("\tRespuesta invalida\n\n");
					goto menu_calidad_post;
				}
				if (strcmp(c,"1")==0){intc=1;}
				else if(strcmp(c,"2")==0){intc=2;}
				else if(strcmp(c,"3")==0){intc=3;}
				else if(strcmp(c,"4")==0){intc=4;}
				else if(strcmp(c,"5")==0){intc=5;}
				else if(strcmp(c,"6")==0){intc=6;}
				else if(strcmp(c,"7")==0){intc=7;}
				else if(strcmp(c,"8")==0){intc=8;}
				else if(strcmp(c,"9")==0){intc=9;}
				else if(strcmp(c,"10")==0){intc=10;}
				t.calidad=intc;							//se escribe la calidad
				}
				//continuar pidiendo datos del post
				p.id_tipo=t.id_tipo;
				strtime=time(NULL); //fecha y hora actual
				timeinfo = localtime(&strtime);
				strftime(p.fecha, 30, "%d/%m/%y %I:%M%p", timeinfo);
				printf("Ingrese el nombre del post:\t"); //imagen se toma como nombre del post
				gets(name);
				strcpy(p.imagen,name);
				printf("Ingrese la leyenda:\t");
				gets(desc);
				strcpy(p.leyenda,desc);
				printf("Escriba una descripcion:\t");
				gets(desc);
				strcpy(p.descripcion,desc);
				p.likes = 0;
				p.dislikes = 0;
				//escritura tipo_temp
				if (fwrite(&t, sizeof(tipo),1, tiptmp)==1){ //usar un while en vez del if
					remove("archivo_tipo.dat");
					if(rename("archivo_tipo_temp.dat","archivo_tipo.dat")!=0){
						printf("Problema al renombrar el archivo tipo\n");
					}
					fclose(tiptmp);
				}
				if (fwrite(&p, sizeof(post),1, postmp)==1){ //usar un while en vez del if
				remove("archivo_post.dat");
				if(rename("archivo_post_temp.dat","archivo_post.dat")==0){
						printf("-----------------------------------------\n");
						printf("Creacion exitosa del post\n");
						printf("-----------------------------------------\n");
					}
					else{
						printf("Problema al renombrar el archivo post\n");
					}
					fclose(postmp);
				}
				goto menu_admin;
				}
		case 5:	modificar_post:{	//editar post... solo las descripciones
				printf("Ingrese nombre del post a modificar: ");
				gets(name);
				if (strcmp(name,"0")==0){
					goto menu_admin;
				}
				int id=buscar_id_post(name, pos);
				if (id==-1){
					printf("el post no existe\n   escriba 0 para volver al menu anterior\n\n");
					goto modificar_post;
				}
				while(copiar_archivo("archivo_post.dat","archivo_post_temp.dat")){
					printf("error al crear el archivo temporal de post\n");
				}
				fseek(pos, id, SEEK_SET);
				fread(&p_original, sizeof(usuario),1, pos); //leyendo los datos antiguos
				//recopilando nuevos datos
				p.id_post=id;
				p.id_admin=p_original.id_admin;
				p.id_tipo=p_original.id_tipo;
				strcpy(p.fecha,p_original.fecha);
				printf("Ingrese nuevo nombre de post: ");
				gets(p.imagen);
				printf("Ingrese nueva leyenda: ");
				gets(p.leyenda);
				printf("Ingrese nueva descripcion: ");
				gets(p.descripcion);
				p.likes=p_original.likes;
				p.dislikes=p_original.dislikes;
				fseek(postmp, id, SEEK_SET);
				if (fwrite(&p, sizeof(usuario),1, postmp)==1){
					remove("archivo_post.dat");
					if(rename("archivo_post_temp.dat","archivo_post.dat")==0){
						printf("-----------------------------------------\n");
						printf("Edicion exitosa de post\n");
						printf("-----------------------------------------\n");
					}
					else{
						printf("Problema al renombrar el archivo temporal\n");
					}
					fclose(postmp);
				}
				else printf("Error de escritura, la modificacion no se llevo a cabo\n");
				goto menu_admin;
				}
		case 6:	borrar_post:{ 		//eliminar post..    (!) falta que se borren los comentarios y el archivo tipo.. actualmente quedan como basura
				printf("Ingrese nombre de post a borrar: ");
				gets(name);
				if (!strcmp("0",name)) goto menu_admin;
				int id=buscar_id_post(name, pos); //donde se reemplazara el ultimo post
				if (id==-1){
					printf("post no encontrado\n");
					goto borrar_post;
				}
				if (borrar_post_temp("archivo_post.dat","archivo_post_temp.dat",id)==0){
					remove("archivo_post.dat");
					if(rename("archivo_post_temp.dat","archivo_post.dat")==0){
						printf("-----------------------------------------\n");
						printf("Eliminacion exitosa de post\n");
						printf("-----------------------------------------\n");
					}
					else{
						printf("Problema al renombrar el archivo temporal\n");
					}
				}
				else printf("Error de escritura, la eliminacion no se llevo a cabo\n");
				goto menu_admin;
				}
		case 7:	break; 				//volver al menu principal
	}
	fclose(usrs);
	fclose(usrstmp);
	fclose(pos);
	fclose(postmp);
	fclose(tip);
	fclose(tiptmp);
	remove("archivo_usuario_temp.dat");
	remove("archivo_post_temp.dat");
	remove("archivo_tipo_temp.dat");
}

int main(){ //menu de login
	char name[30], c[10];
	int intc=0, i, tamanio_archivo_adm, tamanio_archivo_usr;
	admin a; //para la busqueda de admin
	printf(" _    _   _____\n| |  | | |  __ \\ \n| |__| | | |  | |\tJAIDEFINICHON\n|  __  | | |  | |\n| |  | | | |__| |\t\t terrible de calidah\n|_|  |_| |_____/\n\n");
	menu_principal:{
	printf("\n-----------------------------------------\n");
	printf("Menu principal:\n\t1.- Iniciar sesion como usuario\n\t2.- Iniciar sesion como admin\n\t3.- Salir.\nSeleccionar: ");
	gets(c);
	if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0){
		printf("respuesta no valida\n\n");
		goto menu_principal;
	}
	if (strcmp(c,"1")==0){intc=1;}
	else if(strcmp(c,"2")==0){intc=2;}
	else if(strcmp(c,"3")==0){intc=3;}
	}

	FILE *usrs = fopen("archivo_usuario.dat","rb");
	FILE *admn = fopen("archivo_admin.dat","rb");

	if (usrs==NULL || admn==NULL){
		printf("por favor ejecute el programa: 'restaurar_base_de_datos.c' antes de ejecutar este");
		return 0;
	}
	
	switch (intc){
		//ingresar como usuario
		case 1: menu_log_usuario:{
				fseek(usrs, 0, SEEK_END);
				tamanio_archivo_usr=ftell(usrs);
				if (tamanio_archivo_usr==sizeof(usuario)){	//si no se han creado usuarios
					printf("no existen usuarios creados, por favor contactese con un administrador para conseguir una cuenta\n");
					goto menu_principal;
				}
				printf("Ingrese nombre de usuario: ");
				gets(name);
				if (strcmp(name,"0")==0){
					goto menu_principal;
				}
				int id=buscar_id_usuario(name, usrs);
				if (id==-1){
					printf("el usuario no existe\n   escriba 0 para volver al menu anterior\n\n");
					goto menu_log_usuario;
				}
				log_usuario(id);
				goto menu_principal;
				break;
		}
		//ingresar como administrador
		case 2:	menu_log_admin:{
				printf("Ingrese nombre de administrador: ");
				gets(name);
//				fclose(usrs); //intento para que la creacion de cuentas funcione en todo momento
				if (strcmp(name,"0")==0){
					goto menu_principal;
				}
				//Busqueda de administrador en todo el archivo
				fseek(admn, 0, SEEK_END); //para saber el tamaño del archivo (se usa ftell)
				tamanio_archivo_adm = ftell(admn);//en este caso dira que pesa 200 bytes, pero no nesesitamos la direccion del ultimo ya que no deberia haber nada (nesesitamos el penultimo)
				for (i=0; i<=(tamanio_archivo_adm-sizeof(admin)); i+=sizeof(admin)){
					fseek(admn, i, SEEK_SET);
					fread(&a, sizeof(admin), 1, admn);
					if (strcmp(name,(a.nombre))==0){	//administrador encontrado
						log_administrador(a.id_admin);
						goto menu_principal;
					}
					else if (i==(tamanio_archivo_adm-sizeof(admin))){
						printf("el administrador no existe\n   *escriba 0 para volver al menu anterior\n\n");
						goto menu_log_admin;
					}
				}
				break;
		}
		//salir
		case 3:	goto end;
				break;
	}
	goto menu_principal;
	end:
	fclose(usrs);
	fclose(admn);
	printf("\n\t\t\t\tMarchant pls giv 100");
	return 0;
} 
