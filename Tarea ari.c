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
	} usuario;

void mostrar_usuarios(FILE *archivo){ //hacer que estos se ordenen a partir del que posee mas seguidores
	int id, tamanio_archivo;
	usuario u;
	fseek(archivo, 0, SEEK_END);
	tamanio_archivo=ftell(archivo);
	//Busqueda de usuario en todo el archivo
	for (id=sizeof(usuario); id<=(tamanio_archivo-sizeof(usuario)); id+=sizeof(usuario)){
		fseek(archivo, id, SEEK_SET);
		fread(&u, sizeof(usuario), 1, archivo);
		printf("%s\t\tfecha de creacion: %s\n",u.avatar, u.fecha_creacion);
	}
	if (tamanio_archivo==sizeof(usuario)) printf("Actualmente no existen usuarios\n");
	fclose(archivo);
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

int copiar_archivo(char *f_org,char *f_dest){
	FILE *fp_org = fopen(f_org,"rb"),*fp_dest = fopen(f_dest,"wb"); 
	char c;
	int tamanio_origen, tamanio_destino;
	
	if(fp_org==NULL || fp_dest==NULL){
		printf("error al leer o escribir durante la copia\n");
		return 1; //error al leer o escribir
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
	if(tamanio_origen!=tamanio_destino){
//		printf("error: archivos tienen distintos tamanios:\norigen: %d\ndestino: %d\n",tamanio_origen,tamanio_destino);
		return 1; //si los tamaños de los archivos son distintos
	}

//	printf("se crea exitosamente el archivo temp de usuarios\n");
	fclose(fp_org);
	fclose(fp_dest);
	return 0;
}

int borrar_usuario_temp(char *f_org,char *f_dest, int id){ //toma el ultimo registro y lo coloca en la posicion id reemplazando al que estaba en esa posicion
	FILE *fp_org = fopen(f_org,"rb"),*fp_dest = fopen(f_dest,"wb"); 

	usuario u;
	int i, tamanio_origen, tamanio_destino, tamanio_usuario=sizeof(usuario);
	
	if(fp_org==NULL || fp_dest==NULL){
		printf("error al leer o escribir durante la copia\n");
		return 1; //error al leer o escribir
	}
	fseek(fp_org, 0, SEEK_END);
	tamanio_origen=ftell(fp_org);
	//comenzar a copiar todos los parametros antes del ultimo
	for (i=0; i<(tamanio_origen-sizeof(usuario)); i+=sizeof(usuario)){
		fseek(fp_org, i, SEEK_SET);
		if (i==id){
			fseek(fp_org, -tamanio_usuario, SEEK_END); //se posiciona en el principio del ultimo registro de origen
		}
		fread(&u, sizeof(usuario), 1, fp_org);
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

void log_usuario(int id_usuario, FILE *user){
	char c[10];
	int intc=0;
	usuario u, u_original;
	
	printf("\n-----------------------------------------\nPosts actuales:\n");
	// mostrar_posts(fopen("archivo_post.dat","rb"));
	printf("\n-----------------------------------------\n");

	menu_user:{ 																	// mismo formato que el menu principal
		printf("Menu usuario:\n\t1.- Editar Perfil\n\t2.- Seguir a un usuario\n\t3.- Ver un post\n\t4.- Volver.\nSeleccionar: ");
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
	
	FILE *usrs = fopen("archivo_usuario.dat","rb"); 								//ya esta llamado anteriormente
	FILE *usrstmp = fopen("archivo_usuario_temp.dat","wb");
	// FILE *postmp = fopen("archivo_post_temp.dat","ab");   

	switch (intc){
		case 1: modificar_usuario:{												//1.- editar perfil
			printf("\nMenu de edicion:\n\t1.- Editar preferencia\n\t2.- Editar biografia\n\t3.- Volver.\nSeleccionar: ");
			gets(c);
			if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0){
				printf(" Respuesta no valida\n\n");
				goto modificar_usuario;
			}}
		
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
						printf("Edicion de preferencia exitosa\n");					//verificar despues el renombramiento, correcto ==0
						goto modificar_usuario;
					}
					break;
				
				}
				case 2:	{	//nueva biografia
					fseek(usrs, id_usuario, SEEK_SET);								//ir al lugar del usuario
					fread(&u_original, sizeof(usuario),1, usrs);
					printf("Biografia actual:\n\t%s\n-----------------------------------------\n",u_original.bio);				// (!) que imprima la bio anterior, no se si es correcto...
					
					menu_nueva_biografia:{
						printf("ingresar biografía nueva?\n\t1.- Si\n\t2.- Cancelar\nSeleccion: ");
						gets(c);
						
						if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0){
							printf(" Respuesta no valida\n\n");
							goto menu_nueva_biografia;
						}}
					if (strcmp(c,"1")==0){		// ingresar nueva
						printf("-----------------------------------------\ningrese su nueva biografia:\n");
						gets(u.bio);
						printf("\n-----------------------------------------\n");
						
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
							printf("Edicion de biografia exitosa\n-----------------------------------------\n");//verificar despues el renombramiento, correcto ==0
							fclose(usrstmp);
							goto menu_user;
						}
						printf("\nno se guardo biografia D:\n");
					}
					else if(strcmp(c,"2")==0) goto modificar_usuario;
					break;
				
				case 3:	goto menu_user;
						break;			
			}
			}
		case 2:{		// seguir a un usuario
			printf("\n-----------------------------------------\nUsuarios disponibles:\n");
			mostrar_usuarios(fopen("archivo_usuario.dat","rb"));
			printf("\n-----------------------------------------\n");
			ingresar_usuario_a_seguir:{
				printf("Ingrese nombre de usuario a seguir: ");
				gets(c);
				if (!strcmp(c,"0")){
					goto menu_user;
				}
				if (buscar_id_usuario(c, usrs)==-1){
					printf("no se encuentra el nombre, inténtelo nuevamente.\t(ingrese '0' para cancelar)\n");
					goto ingresar_usuario_a_seguir;
					break;
				}}
			
			while(copiar_archivo("archivo_usuario.dat","archivo_usuario_temp.dat")){	//crear temp
				printf("error al crear el archivo temporal de usuarios\n");
			}
			
			fseek(usrs, id_usuario, SEEK_SET);								//ir al lugar del usuario
			fread(&u_original, sizeof(usuario),1, usrs);					// AHAHA
			u.id_usuario_sigue= buscar_id_usuario(c, usrs);
			u.id_usuario=id_usuario;										
			strcpy(u.fecha_creacion,u_original.fecha_creacion);				//pasar datos del original al u
			strcpy(u.avatar,u_original.avatar);								//
			u.preferencia=u_original.preferencia;									//(!) revisar si es correcto
			fseek(usrstmp, id_usuario, SEEK_SET);
			if (fwrite(&u, sizeof(usuario), 1, usrstmp)==1){				//escritura correct
				remove("archivo_usuario.dat");								//borrar original
				rename("archivo_usuario_temp.dat","archivo_usuario.dat"); 	//temp ahora es el original
				printf("#follow registrado\n");					//verificar despues el renombramiento, correcto ==0
				fclose(usrstmp);
				goto menu_user;
			}
			}break;
		case 3: 				// ver posts
		printf("\n-----------------------------------------\nPosts actuales:\nnnnaddah... :3");
		// mostrar_posts(fopen("archivo_post.dat","rb"));
		printf("\n-----------------------------------------\n");
		
		case 4: //volver
			return;			//pa que se cierre no? :s
		
	}
}

void log_administrador(int id_admin, FILE *admn){
	char c[10], name[30], desc[100];
	int intc=0;
	usuario u, u_original;
	post p/*, p_original*/;
	tipo t/*, t_original*/;
	time_t strtime;
	struct tm * timeinfo;
	
	//hacer vista de top 5 de post
	//hacer vista de usuarios existentes (por cualquier admin)------ FALTA ORDENARLOS

	menu_admin:{	//se utiliza el mismo formato que en el menu principal
	printf("\n-----------------------------------------\nUsuarios disponibles:\n");
	mostrar_usuarios(fopen("archivo_usuario.dat","rb"));
	printf("\n-----------------------------------------\n");
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
		case 1:	{//crear usuario
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
				if (fwrite(&u, sizeof(usuario), 1, usrstmp)==1){
					remove("archivo_usuario.dat");
					if(rename("archivo_usuario_temp.dat","archivo_usuario.dat")==0){
						printf("Creacion exitosa de usuario\n");
					}
					else{
						printf("Problema al renombrar el archivo temporal\n");
					}
					fclose(usrstmp);
				}
				goto menu_admin;
				}
		case 2:	modificar_usuario:{ //solo modifica el nombre de usuario y su preferencia
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
				fseek(usrstmp, id, SEEK_SET);
				if (fwrite(&u, sizeof(usuario),1, usrstmp)==1){
					remove("archivo_usuario.dat");
					if(rename("archivo_usuario_temp.dat","archivo_usuario.dat")==0){
						printf("Edicion exitosa de usuario\n");
					}
					else{
						printf("Problema al renombrar el archivo temporal\n");
					}
					fclose(usrstmp);
				}
				else printf("Error de escritura, la modificacion no se llevo a cabo\n");
				goto menu_admin;
				}
		case 3:	borrar_usuario:{ //eliminar usuario
				printf("Ingrese nombre de usuario a borrar: ");
				gets(name);
				int id=buscar_id_usuario(name, usrs); //donde se reemplazara el ultimo usuario
				if (id==-1){
					printf("usuario no encontrado\n");
					goto borrar_usuario;
				}
				if (borrar_usuario_temp("archivo_usuario.dat","archivo_usuario_temp.dat",id)==0){
					remove("archivo_usuario.dat");
					if(rename("archivo_usuario_temp.dat","archivo_usuario.dat")==0){
						printf("Eliminacion exitosa de usuario\n");
					}
					else{
						printf("Problema al renombrar el archivo temporal\n");
					}
				}
				else printf("Error de escritura, la eliminacion no se llevo a cabo\n");
				goto menu_admin;
				}
		case 4: {//crear post
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
				p.id_tipo=ftell(tiptmp);
				//datos archivo tipo
				t.id_tipo=p.id_tipo;
				menu_tipo_post:{ //peticion del tipo de post
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
				printf("Calidad del tipo del post (1-10): ");
				gets(c);
				if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0 && strcmp(c,"4")!=0 && strcmp(c,"5")!=0 && strcmp(c,"6")!=0 && strcmp(c,"7")!=0 && strcmp(c,"8")!=0 && strcmp(c,"9")!=0 && strcmp(c,"10")!=0){
					printf(" Respuesta no valida\n\n");
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
				t.calidad=intc;
				}
				if (fwrite(&t, sizeof(usuario),1, tiptmp)==1){ //usar un while en vez del if
					remove("archivo_tipo.dat");
					if(rename("archivo_tipo_temp.dat","archivo_tipo.dat")!=0){
						printf("Problema al renombrar el archivo tipo\n");
					}
					fclose(usrstmp);
				}
				//continuar pidiendo datos del post
				strtime=time(NULL); //fecha y hora actual
				timeinfo = localtime(&strtime);
				strftime(p.fecha, 30, "%d/%m/%y %I:%M%p", timeinfo);
				printf("Ingrese el nombre del post: "); //imagen se toma como nombre del post
				gets(name);
				strcpy(p.imagen,name);
				printf("Ingrese la leyenda: ");
				gets(desc);
				strcpy(p.leyenda,desc);
				printf("Escriba una descripcion: ");
				gets(desc);
				strcpy(p.descripcion,desc);
				p.likes = 0;
				p.dislikes = 0;
				if (fwrite(&p, sizeof(usuario),1, postmp)==1){ //usar un while en vez del if
				remove("archivo_post.dat");
				if(rename("archivo_post_temp.dat","archivo_post.dat")==0){
						printf("Creacion exitosa del post\n");
					}
					else{
						printf("Problema al renombrar el archivo post\n");
					}
					fclose(postmp);
				}
				goto menu_admin;
				}
		case 5:	break; //editar post
		case 6:	break; //eliminar post
		case 7:	break; //simplemente termina, por lo que te lleva al menu principal
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
		printf("por favor ejecute el programa: 'creador_archivos.c' antes de ejecutar este");
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
				log_usuario(id, usrs);
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
						log_administrador(a.id_admin, admn);
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
	printf("\naaandate vo zarpao qlo lomgii y ke pazaaa");
	return 0;
}
