
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <crypt.h>
#include <mpi.h>

/*****************************************************************************
The variable names and the function names of this program is same as provided by the university.
The added variable and function are the only changes made to this program.
  

To compile:
     mpicc -o SantosPW SantosPW.c -lrt -lcrypt
     
  To run 3 processes on this computer:
    mpirun -n 3 ./SantosPW >result.txt
*****************************************************************************/


int n_passwords = 4;
pthread_t t1, t2;
char *encrypted_passwords[] = {
"$6$KB$eOk5f/THD7xQr6vKniSca/DdqY.m.gCg6Z.4zNWqD7V4yjkDuwCm2v5raQsqMcDEg344dywrZiRLhY65j4.Wg0",
  "$6$KB$3LN07V7OD08Er.Ll65X7XKJrGTa/ae8vqmEWXpoFrlVFdKf6uZr5mknez0ony6kFk.bxRuzhXKJBdCNTrI6Ve0",
  "$6$KB$zVovTonYa566ysxOHeowsIG2v.RKReAJfmNuyYwUGoOy6o29dO.eIHV6J68a4VJA1sPcKV/CojnC3lqluJnNF.",
  "$6$KB$5JX4XT.crABQeRSeWUyjmw8kMftxhH0gsgJeR99Vhqw5xY.StnlRCmEAGxNfxPo8S1uyGW3tpKyB6/2j/Y3GC0"
};

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}
  
void kernel_function1(char *salt_and_encrypted){
  int p, q, r;     
  char salt[7];    

  char plain[7];   
  char *enc;       
  int count = 0;   

  substr(salt, salt_and_encrypted, 0, 6);
  
  for(p='A'; p<='M'; p++){
    for(q='A'; q<='Z'; q++){
      for(r=0; r<=99; r++){
	//printf("Instance 1:");
	sprintf(plain, "%c%c%02d",p, q, r);
	enc = (char *) crypt(plain, salt);
	count++;
	if(strcmp(salt_and_encrypted, enc) == 0){
	  printf("#%-8d%s %s\n", count, plain, enc);
	} else {
	  printf(" %-8d%s %s\n", count, plain, enc);
	}
      }
    }
  }
  printf("%d solutions explored\n", count);
}
void kernel_function2(char *salt_and_encrypted){
  int p, q, r;     
  char salt[7];    

  char plain[7];   
  char *enc;       
  int count = 0;  

  substr(salt, salt_and_encrypted, 0, 6);
  
  for(p='N'; p<='Z'; p++){
    for(q='A'; q<='Z'; q++){
      for(r=0; r<=99; r++){
	//printf("Instance 2:");
	sprintf(plain, "%c%c%02d",p, q, r);
	enc = (char *) crypt(plain, salt);
	count++;
	if(strcmp(salt_and_encrypted, enc) == 0){
	  printf("#%-8d%s %s\n", count, plain, enc);
	} else {
	  printf(" %-8d%s %s\n", count, plain, enc);
	}
      }
    }
  }
  printf("%d solutions explored\n", count);
}


int time_difference(struct timespec *start, struct timespec *finish,
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

int main(int argc, char** argv) {
 struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);

 
  int size, rank;
int i;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(size != 3) {
    if(rank == 0) {
      printf("This program needs to run on exactly 3 processes\n");
    }
  } else {
    if(rank ==0){

      int x;
    
      MPI_Send(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      MPI_Send(&x, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);


 
    } else if(rank==1){
      int number;
      MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, 
                         MPI_STATUS_IGNORE);
        for(i=0;i<n_passwords;i<i++) {
          kernel_function1(encrypted_passwords[i]);
        }
      }
      else{
      int number;
      MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, 
                         MPI_STATUS_IGNORE);
        for(i=0;i<n_passwords;i<i++) {
          kernel_function2(encrypted_passwords[i]);
        }
      }
    }
    MPI_Finalize(); 
 clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
         (time_elapsed/1.0e9)); 

  return 0;
}
