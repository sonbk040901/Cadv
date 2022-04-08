#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define maxElements 1000
#define maxLength 2000

int preProcessorString(char inStr[maxLength], char outStr[maxLength])
{
    //ham tien xu li chuoi s trong do bo di cac dau cau nh , .:;"'() va cac ki tu dac biet
    //ham se chi giu lai cac ki tu alphabet tieu chuan la so va cac chu cai Latinh
    //tra ve 1 neu thuc hien thanh cong
    //tra ve 0 neu thuc hien that bai
    //inStr: chuoi goc chua xu li
    //outStr: chuoi ket qua da tien xu li bo cac ki tu dac biet
    int result = 0;
    strcpy(outStr, ""); //reset chuoi output, phong truong hop truoc do da co gia tri
    //0-9: 48-57 trong ASCII table
    //A-Z: 65-90
    //a-z: 97-122
    int i = 0;
    int j = 0;
    for (i = 0; i < strlen(inStr); i++)
    {
        if (!(((inStr[i] < 48) && (inStr[i] != 32)) || ((inStr[i] > 57) && (inStr[i] < 65)) || ((inStr[i] > 90) && (inStr[i] < 97)) || (inStr[i] > 122)))
        {
            outStr[j] = inStr[i];
            j++;
        }
    }
    return result;
}

int main()
{
    char inputString[maxLength] = "";
    char newString[10][10];
    printf("===-WORD RECOMMENDATION SYSTEMS.-===\n");
    printf("Moi ban nhap vao mot chuoi.Ket thuc viec thong ke bang #.#\n");
    while (1)
    {
        char sTemp[maxLength] = "";
        printf("Chuoi nhap:");
        fflush(stdin);
        scanf("%[^\n]", sTemp);
        if ((strcmp(sTemp, "") != 0) && (strcmp(sTemp, "#.#") != 0))
        {
            strcpy(inputString, "");
            strcpy(inputString, sTemp);
            //			strcpy(inputString,"Hello C programming language world.  C is the most professional powerful programming language.");
            printf("Ban vua nhap chuoi \"%s\"\n", inputString);
        }
        else
        {
            break;
        }

        printf("--------------------------------------------\n");

        int i, j = 0, str = 0;
        for (i = 0; i <= strlen(sTemp); i++)
        {
            if (sTemp[i] == ' ' || sTemp[i] == '\0')
            {
                newString[str][j] = '\0';
                str++;
                j = 0;
            }
            else
            {
                newString[str][j] = sTemp[i];
                j++;
            }
        }
        printf("XU LI TACH CHUOI THEO DAU CACH.\n");
        for (i = 0; i < str; i++)
        {
            printf(" %s\n", newString[i]);
        }

        printf("----------------------------------------------------\n");

        printf("DEM SO LAN XUAT HIEN CUA CAC PHAN TU.\n");
        int count = 0;
        int dem[100];
        char arr[10][10];
        for (int i = 0; i < 100; i++)
        {
            dem[i] = 1;
        }
        for (i = 0; i < str; i++)
        {
            for (j = 0; j <= count; j++)
            {
                if (strcmp(arr[j], newString[i]) == 0)
                {
                    dem[j]++;
                    break;
                }
            }
            if (j == count + 1)
            {
                count++;
                strcpy(arr[j], newString[i]);
            }
        }
        for (int i = 1; i <= count; i++)
        {
            printf("%s: %d\n", arr[i], dem[i]);
        }
    }
    return 0;
}