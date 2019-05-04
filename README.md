# SoalShift_modul4_F10

## Jawaban Soal Shift Sistem Operasi 2019 

## Modul 4

Oleh: 

* 05111640000125 Fadhlan Aulia
* 05111740000019 Paramastri Ardiningrum


### 1. 

Semua nama file dan folder harus terenkripsi

Enkripsi yang Atta inginkan sangat sederhana, yaitu Caesar cipher. Namun, Kusuma mengatakan enkripsi tersebut sangat mudah dipecahkan. Dia menyarankan untuk character list diekspansi,tidak hanya alfabet, dan diacak. Berikut character list yang dipakai:

``qE1~ YMUR2"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\8s;g<{3.u*W-0``

Misalkan ada file bernama “halo” di dalam folder “INI_FOLDER”, dan key yang dipakai adalah 17, maka:

“INI_FOLDER/halo” saat belum di-mount maka akan bernama “n,nsbZ]wio/QBE#”, saat telah di-mount maka akan otomatis terdekripsi kembali menjadi “INI_FOLDER/halo” .

Perhatian: Karakter ‘/’ adalah karakter ilegal dalam penamaan file atau folder dalam NIX, maka dari itu dapat diabaikan


### Solusi

* Kita inisialisasi daftar karakter yang tersedia untuk menjadi nama file yang nantinya di enkripsi dan dekripsi. 
* Setelah itu buat fungsi enskripsi dan dekripsi
* Kedua fungsi tersebut dibuat dengan aturan caesar cipher
* Node . dan .. dipastikan tidak akan terenkripsi
* Fungsi akan memeriksa satu persatu karakter yang akan dienkripsi apakah ada dalam daftar karakter yang tersedia
* Akan terjadi perulangan sebanyak panjang karakter yang dienkripsi dan sepanjang karakter yang tersedia
* Untuk enkripsi, kita gunakan rumus enkripsi ``(x + key) mod 94`` dan dekripsi ``(x - key) mod 94`` tetapi disini saya menggunakan rumus lain untuk dekripsi yang namun dapat diartikan sama yaitu ``(x + (94-key)) mod 94``
* Fungsi enc atau enkripsi akan dipanggil di setiap fungsi xmp yang dipakai untuk program fuse ini

```
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
```

```
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
}
```

![alt text](https://github.com/paramastri/SoalShift_modul4_F10/blob/master/1.PNG)

### 2. 

Semua file video yang tersimpan secara terpecah-pecah (splitted) harus secara otomatis tergabung (joined) dan diletakkan dalam folder “Videos”

Urutan operasi dari kebutuhan ini adalah:

a. Tepat saat sebelum file system di-mount
* Secara otomatis folder “Videos” terbuat di root directory file system
* Misal ada sekumpulan file pecahan video bernama “computer.mkv.000”, “computer.mkv.001”, “computer.mkv.002”, dst. Maka secara otomatis file pecahan tersebut akan di-join menjadi file video “computer.mkv”
Untuk mempermudah kalian, dipastikan hanya video file saja yang terpecah menjadi beberapa file. File pecahan tersebut dijamin terletak di root folder fuse
* Karena mungkin file video sangat banyak sehingga mungkin saja saat menggabungkan file video, file system akan membutuhkan waktu yang lama untuk sukses ter-mount. Maka pastikan saat akan menggabungkan file pecahan video, file system akan membuat 1 thread/proses(fork) baru yang dikhususkan untuk menggabungkan file video tersebut
* Pindahkan seluruh file video yang sudah ter-join ke dalam folder “Videos”
* Jangan tampilkan file pecahan di direktori manapun
b. Tepat saat file system akan di-unmount
* Hapus semua file video yang berada di folder “Videos”, tapi jangan hapus file pecahan yang terdapat di root directory file system
* Hapus folder “Videos” 

### 3. 

Sebelum diterapkannya file system ini, Atta pernah diserang oleh hacker LAPTOP_RUSAK yang menanamkan user bernama “chipset” dan “ic_controller” serta group “rusak” yang tidak bisa dihapus. Karena paranoid, Atta menerapkan aturan pada file system ini untuk menghapus “file bahaya” yang memiliki spesifikasi:

* Owner Name 	: ‘chipset’ atau ‘ic_controller’
* Group Name	: ‘rusak’
* Tidak dapat dibaca

Jika ditemukan file dengan spesifikasi tersebut ketika membuka direktori, Atta akan menyimpan nama file, group ID, owner ID, dan waktu terakhir diakses dalam file “filemiris.txt” (format waktu bebas, namun harus memiliki jam menit detik dan tanggal) lalu menghapus “file bahaya” tersebut untuk mencegah serangan lanjutan dari LAPTOP_RUSAK.

### Solusi

* Kita buat struct untuk stat username dan group nya
* Kita beri kondisi ketika username nya chipset atau ic_controller, serta groupname nya rusak dan file tersebut tidak dapat dibaca
* Apabila memenuhi kondisi tadi, kita buat filemiris.txt yang akan merekam seluruh file dan folder yang terhapus karena terdeteksi sesuai dengan kondisi di atas
* Untuk pencatatan waktu, kami gunakan variabel time_t dan time(NULL) yang menunjukkan waktu terakhir diaksesnya file/folder tersebut
* Ada perintah remove(file) untuk menghapus file terindikasi.

Pada fungsi xmp_readdir:

```
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
```
![alt text](https://github.com/paramastri/SoalShift_modul4_F10/blob/master/3.PNG)

### 4. 

Pada folder YOUTUBER, setiap membuat folder permission foldernya akan otomatis menjadi 750. Juga ketika membuat file permissionnya akan otomatis menjadi 640 dan ekstensi filenya akan bertambah “.iz1”. File berekstensi “.iz1” tidak bisa diubah permissionnya dan memunculkan error bertuliskan “File ekstensi iz1 tidak boleh diubah permissionnya.”

### Solusi

Membuat folder:

```
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
```

Membuat file:

```
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
```

Melarang file yang akan diubah permissionnya:

```
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
```

![alt text](https://github.com/paramastri/SoalShift_modul4_F10/blob/master/4.PNG)

### 5. 

Ketika mengedit suatu file dan melakukan save, maka akan terbuat folder baru bernama Backup kemudian hasil dari save tersebut akan disimpan pada backup dengan nama namafile_[timestamp].ekstensi. Dan ketika file asli dihapus, maka akan dibuat folder bernama RecycleBin, kemudian file yang dihapus beserta semua backup dari file yang dihapus tersebut (jika ada) di zip dengan nama namafile_deleted_[timestamp].zip dan ditaruh kedalam folder RecycleBin (file asli dan backup terhapus). Dengan format [timestamp] adalah yyyy-MM-dd_HH:mm:ss
