 #include<stdio.h> 
 #include<stdlib.h> 
 #include<string.h> 
  
  
 void func(char ** argv) 
 { 
     char arr[16]; 
     strcpy(arr, argv[2]); 
  
     return; 
 } 
  
 int main(int argc, char *argv[]) 
 { 
     func(argv); 
     return 0; 
 }
