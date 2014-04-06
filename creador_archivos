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

typedef struct admins{
	int id_admin;
	char nombre[30];
	unsigned int edad;
	} admin;

typedef struct usuarios{
	int id_usuario;
	int id_usuario_sigue;
	char fecha_creacion[30];
	enum tipo_post preferencia;
	char avatar[30];
	char bio[100];
	} usuario;

void crear_user_default(){ //creando al usuario nadie que todos siguen
	time_t tiempo;
	struct tm *timeinfo;
	FILE *usrs = fopen("archivo_usuario.dat","wb");
	usuario u;
	rewind(usrs);
	u.id_usuario=0;
	u.id_usuario_sigue=0;
	tiempo= time(NULL);
	timeinfo = localtime(&tiempo); //fecha y hora actual
	strftime(u.fecha_creacion, 30, "%d/%m/%y %I:%M%p", timeinfo);
	u.preferencia = graves;		//cualquier wea
	strcpy(u.avatar,"no esta siguiendo a nadie");
	strcpy(u.bio,"este usuario no esta siguiendo a ningun usuario");
	fwrite(&u, sizeof(usuario),1, usrs);
	fclose(usrs);
	printf("escritura de usuario 'nadie' en 'archivo_usuario.dat' exitosa\n");
}

void crear_admins(){
	FILE *adm = fopen("archivo_admin.dat","wb");
	//datos administradores a agregar
	admin a,b,c,d,e,f,g;
	rewind(adm);	//es lo mismo que fseek(adm, 0, SEEK_SET);... lleva al inicio
	int id_marchant=ftell(adm); //en este caso es 0
	a.id_admin=id_marchant;
	strcpy(a.nombre,"Carlos Marchant");
	a.edad=22;
	//siguiente admin:
	fseek(adm, id_marchant+sizeof(admin), SEEK_SET);	//almacenamiento contiguo
	int id_mayastation=ftell(adm);
	b.id_admin=id_mayastation;
	strcpy(b.nombre,"Mayastation");
	b.edad=20;
	fseek(adm, id_mayastation+sizeof(admin), SEEK_SET);
	int id_niko_tesla=ftell(adm);
	c.id_admin=id_niko_tesla;
	strcpy(c.nombre,"Niko Tesla");
	c.edad=35;
	fseek(adm, id_niko_tesla+sizeof(admin), SEEK_SET);
	int id_nico_taringa=ftell(adm);
	d.id_admin=id_nico_taringa;
	strcpy(d.nombre,"Nico Taringa");
	d.edad=99;
	fseek(adm, id_nico_taringa+sizeof(admin), SEEK_SET);
	int id_camarox=ftell(adm);
	e.id_admin=id_camarox;
	strcpy(e.nombre,"Camarox");
	e.edad=777;
	//admins nuevo
	fseek(adm, id_camarox+sizeof(admin), SEEK_SET);
	int id_jorjoql=ftell(adm);
	f.id_admin=id_jorjoql;
	strcpy(f.nombre,"jorjoql");
	f.edad=1313;
	fseek(adm, id_jorjoql+sizeof(admin), SEEK_SET);
	int id_rafaql=ftell(adm);
	g.id_admin=id_rafaql;
	strcpy(g.nombre,"rafaql");
	g.edad=1313;
	//	escritura de administradores
	rewind(adm);
	fwrite(&a, sizeof(admin),1, adm);
	fseek(adm, id_mayastation, SEEK_SET);
	fwrite(&b, sizeof(admin),1, adm);
	fseek(adm, id_niko_tesla, SEEK_SET);
	fwrite(&c, sizeof(admin),1, adm);
	fseek(adm, id_nico_taringa, SEEK_SET);
	fwrite(&d, sizeof(admin),1, adm);
	fseek(adm, id_camarox, SEEK_SET);
	fwrite(&e, sizeof(admin),1, adm);
	fseek(adm, id_jorjoql, SEEK_SET);
	fwrite(&f, sizeof(admin),1, adm);
	fseek(adm, id_rafaql, SEEK_SET);
	fwrite(&g, sizeof(admin),1, adm);
	fclose(adm);
	printf("escritura de administradores en 'archivo_admin.dat' exitosa\n");
}

int main(){
	crear_user_default();
	crear_admins();
//-------------------------------------------------//
//---------probando la lectura de archivos---------//
//-------------------------------------------------//
// /*
	printf("\nprobando la lectura de archivo admin:\n");
	FILE *archivo=fopen("archivo_admin.dat","rb");
	FILE *user=fopen("archivo_usuario.dat","rb");
	admin nesesitado;
	usuario nadie;
	int administrador=2; //0: marchant, 1:Mayastation, 2: Niko Tesla... en la practica se tendra su id por lo q esto no sera nesesario
	int id=administrador*sizeof(admin);
	fseek(archivo, id, SEEK_SET);
	fread(&nesesitado, sizeof(admin), 1, archivo);
	printf("se encuentra al admin %s\n",nesesitado.nombre);
	rewind(user);
	fread(&nadie, sizeof(usuario), 1, user);
	printf("fecha creacion usuario nadie: %s\n",nadie.fecha_creacion);
	fclose(archivo);
	fclose(user);
	return 0;
// */
}
