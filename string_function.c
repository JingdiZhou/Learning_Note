#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

//把 src 所指向的字符串复制到 dest，注意：dest定义的空间应该⽐src⼤
char* strcpy(char*dest,const char*src){
    char*ret=dest;
    assert(dest!=NULL);//检查输入参数
    assert(src!=NULL);
    while(*src!='\0')
        *(dest++)=*(src++);
    *dest='\0';
    return ret;
}


//把 src 所指向的字符串追加到 dest 所指向的字符串的结尾
char* strcat(char*dest,const char*src){
    //1. 将⽬的字符串的起始位置先保存，最后要返回它的头指针
 //2. 先找到dest的结束位置,再把src拷⻉到dest中，记得在最后要加上'\0'
    char*ret=dest;
    assert(dest!=NULL);
    assert(src!=NULL);
    while(*dest!='\0')
        dest++;
    while(*src!='\0')
        (*dest++)=(*src++);
    *dest='\0';
    return ret;
}


//把 str1 所指向的字符串和 str2 所指向的字符串进⾏⽐较。
//该函数返回值如下：
//如果返回值 < 0，则表示 str1 ⼩于 str2。
//如果返回值 > 0，则表示 str1 ⼤于 str2。
//如果返回值 = 0，则表示 str1 等于 str2。
int strcmp(const char*s1,const char*s2){
    assert(s1!=NULL);
    assert(s2!=NULL);
    while(*s1!='\0'&&*s2!='\0'){
        if(*s1>*s2)
            return 1;
        else if(*s1<*s2)
            return -1;
        else 
            s1++; s2++;
    }

    if(*s1>*s2)
        return 1;
    else if(*s1<*s2)
        return -1;
    else
        return 0;
}


char*strstr(char*str1,char*str2){
    char* s = str1;
    assert(str1!='\0');
    assert(str2!='\0');
    if(*str2=='\0')
        return NULL;//若str2为空，则直接返回空
    while(*s!='\0') {//若不为空，则进⾏查询
        char* s1 = s;
        char* s2 = str2;
        while(*s1!='\0'&&*s2!='\0' && *s1==*s2)
            s1++,s2++;
        if(*s2=='\0')
            return str2;//若s2先结束
        if(*s2!='\0' && *s1=='\0')
            return NULL;//若s1先结束⽽s2还没结束，则返回空
        s++;
    }
    return NULL;
}
