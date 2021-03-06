#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <grp.h>
#include <pwd.h>


static const char *dirpath = "/home/paramastri/shift4";

char key[97] = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";

void enc(char* char_enc)
{
	if(!strcmp(char_enc,".") || !strcmp(char_enc,"..")) // spy tdk enc node
	return;
	for(int i=0;i<strlen(char_enc);i++)
	{
		for(int j=0;j<94;j++){
			if(char_enc[i]==key[j]){
				char_enc[i] = key[(j+17)%94];
				break;
			}
		}
	}
}

void dec(char* char_dec)
{
	if(!strcmp(char_dec,".") || !strcmp(char_dec,"..")) return;
	for(int i=0;i<strlen(char_dec);i++)
	{
		for(int j=0;j<94;j++){
			if(char_dec[i]==key[j]){
				char_dec[i] = key[(j+77)%94];
				break;
			}
		}
	}
}


static int xmp_getattr(const char *path, struct stat *stbuf)
{
	int res;
    char fpath[1000];
	char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name); //ngeprint dirpath & name ke fpath
	res = lstat(fpath, stbuf); //nge ls stat yang fpath
	if (res == -1)
		return -errno;

	return 0;
}


static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);
	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) { //baca per file di shift4
		struct stat st;
        char katak[100000];
		strcpy(katak,de->d_name);
		char file[1000];
		sprintf(file,"%s%s",fpath,katak);
		dec(katak);

		struct stat tmp;
		stat(file,&tmp);
		struct passwd *name = getpwuid(tmp.st_uid);
    	struct group *grup = getgrgid(tmp.st_gid);
		
		if( (strcmp(name->pw_name,"chipset") == 0 || strcmp(name->pw_name,"ic_controller") == 0) 
			&& strcmp(grup->gr_name,"rusak")==0 
			&& ((tmp.st_mode & R_OK) == 0) )
		{
			printf("%s\n",file);
			char root[1000];
			strcpy(root,dirpath);
			char note[10000] = "/filemiris.txt";
			enc(note);
			strcat(root,note);
			FILE * fp;
   			fp = fopen (root, "a+");
			char t[1000];
			time_t now = time(NULL);
			strftime(t, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
			char buffer[1000];
			sprintf(buffer,"%s%s-%d-%d-%s",path,katak,name->pw_uid,grup->gr_gid,t);
			fprintf(fp,"%s\n",buffer);
			remove(file);
			fclose(fp);
			chown(root,1000,1000);
		}
		else{
			memset(&st, 0, sizeof(st));
			st.st_ino = de->d_ino;
			st.st_mode = de->d_type << 12;
			if (filler(buf, katak, &st, 0))
				break;
		}
	}

	closedir(dp);
	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int res;

	char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);
	if (S_ISREG(mode)) {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(fpath, mode);
	else
		res = mknod(fpath, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}

//buat folder
static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
	if(strlen(name)>9 && strncmp(name,"/YOUTUBER",9)==0) // bahwa foldernya di dlm youtuber
	{
		enc(name);
		sprintf(fpath, "%s%s",dirpath,name);
		res = mkdir(fpath, 0750);	
	}
	else{
    	enc(name);
		sprintf(fpath, "%s%s",dirpath,name);
		res = mkdir(fpath, mode);
		if (res == -1)
			return -errno;
	}
	return 0;
}

//buat file
static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi)
{

    (void) fi;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    int res;
	if(strlen(name)>9 && strncmp(name,"/YOUTUBER",9)==0)
	{
		strcat(name,".iz1");
		enc(name);
		sprintf(fpath, "%s%s",dirpath,name);
    	res = creat(fpath, 0640);
	}
	else{
    	enc(name);
		sprintf(fpath, "%s%s",dirpath,name);
    	res = creat(fpath, mode);
	}
    if(res == -1)
	return -errno;

    close(res);

    return 0;
}

static int xmp_unlink(const char *path)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
	
    	enc(name);
		sprintf(fpath, "%s%s",dirpath,name);
		res = unlink(fpath);
		if (res == -1)
			return -errno;
	
	return 0;
}

static int xmp_rmdir(const char *path)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);
	res = rmdir(fpath);
	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
	int res;
    char new_from[1000];
    char new_to[1000];
	char name1[1000];
	char name2[1000];
	sprintf(name1,"%s",from);
	sprintf(name2,"%s",to);
	enc(name1);
	enc(name2);
    sprintf(new_from,"%s%s",dirpath,name1);
    sprintf(new_to,"%s%s",dirpath,name2);
	res = rename(new_from, new_to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
	if(strcmp(name+strlen(name)-4,".iz1")==0)
	{
		pid_t child1;
		child1=fork();
		if(child1==0){
			execl("/usr/bin/zenity","/usr/bin/zenity","--error","--text=File ekstensi iz1 tidak boleh diubah permissionnya.","--title=Tidak bisa merubah",NULL);
		}
		else{
			wait(NULL);
		}
	}
	else{
    	enc(name);
		sprintf(fpath, "%s%s",dirpath,name);
		res = chmod(fpath, mode);
	}
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);
	res = lchown(fpath, uid, gid);
	if (res == -1) 
		return -errno;

	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);
	res = truncate(fpath, size);
	if (res == -1)
		return -errno;

	return 0;
}

//biar bs create file
static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	int res;
	struct timeval tv[2];

	char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(fpath, tv);
	if (res == -1)
		return -errno;

	return 0;
}
static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);
	res = open(fpath, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	int fd;
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);
	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;
    char fpath[1000];
    char name[1000];

	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath,"%s%s",dirpath,name);

	char newname[1000];
	char folder[1000] = "";
	enc(folder);
	char folderdir[1000];
	sprintf(folderdir,"%s%s",dirpath,folder);
	mkdir(folderdir,0755);

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;
	printf("%s\n",buf);
	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);

	struct stat sd;
	if(stat(fpath,&sd)>-1){
		char t[1000];
		time_t now = time(NULL);
		strftime(t, 22, "_%Y-%m-%d_%H:%M:%S", localtime(&now));
		dec(name);
		sprintf(newname,"",name,t);
		enc(newname);
		memset(fpath,'\0',sizeof(fpath));
		sprintf(fpath,"%s%s",dirpath,newname);
		// printf("%s\n",fpath);
	
		FILE *fptr = fopen(fpath, "w+"); 
		fprintf(fptr,"%s",buf);
		fclose(fptr);

		return res;
	}
	
	return res;
}




static int xmp_release(const char *path, struct fuse_file_info *fi)
{
	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */

	char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);

	(void) fpath;
	(void) fi;
	return 0;
}

static int xmp_access(const char *path, int mask)
{
	int res;

	char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath, "%s%s",dirpath,name);

	res = access(fpath, mask);
	if (res == -1)
		return -errno;

	return 0;
}


static struct fuse_operations xmp_oper = 
{
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.mknod		= xmp_mknod,
	.mkdir		= xmp_mkdir,
	.unlink		= xmp_unlink,
	.rmdir		= xmp_rmdir,
	.access		= xmp_access,
	.rename		= xmp_rename,
	.chmod		= xmp_chmod,
	.chown		= xmp_chown,
	.truncate	= xmp_truncate,
	.utimens	= xmp_utimens,
	.open		= xmp_open,
	.read		= xmp_read,
	.write		= xmp_write,
	.create		= xmp_create,
	.release	= xmp_release,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
