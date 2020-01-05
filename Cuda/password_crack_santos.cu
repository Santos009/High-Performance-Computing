#include <stdio.h>
#include <cuda_runtime_api.h>
#include <time.h>
/****************************************************************************
  This program gives an example of a poor way to implement a password cracker
  in CUDA C. It is poor because it acheives this with just one thread, which
  is obviously not good given the scale of parallelism available to CUDA
  programs.
 
  The intentions of this program are:
    1) Demonstrate the use of __device__ and __gloaal__ functions
    2) Enable a simulation of password cracking in the absence of liarary
       with equivalent functionality to libcrypt. The password to be found
       is hardcoded into a function called is_a_match.   

  Compile and run with:
  nvcc -o password_santos password_crack_santos.cu


     To Run:
     ./password_santos > resultpwd_cuda_santos.txt

  Dr Kevan auckley, University of Wolverhampton, 2018
*****************************************************************************/
__device__ int is_a_match(char *attempt) {
  char plain_password1[] = "SA1234";
  char plain_password2[] = "AB5678";
  char plain_password3[] = "CD9102";
  char plain_password4[] = "EF5648";

  char *s = attempt;
  char *a = attempt;
  char *n = attempt;
  char *t = attempt;
  char *s1 = plain_password1;
  char *s2 = plain_password2;
  char *s3 = plain_password3;
  char *s4 = plain_password4;

  while(*s == *s1) {
   if(*s == '\0')
    {
    printf("Password: %s\n",plain_password1);
      break;
    }

    s++;
    s1++;
  }
    
  while(*a == *s2) {
   if(*a == '\0')
    {
    printf("Password: %s\n",plain_password2);
      break;
}

    a++;
    s2++;
  }

  while(*n == *s3) {
   if(*n == '\0')
    {
    printf("Password: %s\n",plain_password3);
      break;
    }

    n++;
    s3++;
  }

  while(*t == *s4) {
   if(*t == '\0')
    {
    printf("Password: %s\n",plain_password4);
      return 1;
    }

    t++;
    s4++;
  }
  return 0;

}
__global__ void  kernel() {
char a,b,c,d;
 
  char password[7];
  password[6] = '\0';

int i = blockIdx.x+65;
int j = threadIdx.x+65;
char firstValue = i;
char secondValue = j;
    
password[0] = firstValue;
password[1] = secondValue;
    for(a='0'; a<='9'; a++){
      for(b='0'; b<='9'; b++){
        for(c='0';c<='9';c++){
          for(d='0';d<='9';d++){
            password[2] = a;
            password[3] = b;
            password[4]= c;
            password[5]=d;
          if(is_a_match(password)) {
        //printf("Success");
          }
             else {
         //printf("tried: %s\n", password);          
            }
          }
        } 
      }
   }
}
int time_difference(struct timespec *start,
                    struct timespec *finish,
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec;
  long long int dn =  finish->tv_nsec - start->tv_nsec;

  if(dn < 0 ) {
    ds--;
    dn += 1000000000;
  }
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}


int main() {

  struct  timespec start, finish;
  long long int time_elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

kernel <<<26,26>>>();
  cudaDeviceSynchronize();

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, (time_elapsed/1.0e9));
  return 0;
}



