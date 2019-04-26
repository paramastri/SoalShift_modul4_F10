#include <stdio.h>
#include <math.h>
#include <string.h>
void main()
{
    int k,j,i,h,u=0;
    char katanya[100];
    char p[100],c[100];
    char listnya[100]="qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";


    printf("Masukkan kata: ");
    gets(katanya);
    printf("Key: ");
    scanf("%d",&k);

    for(i=0;i<strlen(katanya);i++)
    {
        if(katanya[i]==' ')
        {
            i++;
            u++;
        }
        for(j=0;j<strlen(listnya);j++)
        {
            if(listnya[j]==katanya[i])
            {
                h=(j+k)%26;
                c[i-u]=listnya[h];
                break;
            }
        }
    }

    c[i-u]='\0';
    printf("\nHasil enkripsi: ");
    puts(c);

    for(i=0;i<strlen(c);i++)
    {
        for(j=0;j<strlen(listnya);j++)
        {
            if(listnya[j]==c[i])
            {
                h=(j-k)%26;
                p[i]=listnya[h];
                break;
            }
        }
    }

    p[i]='\0';
    printf("Hasil dekripsi: ");
    puts(p);
//    getch();
    return 0;
}
