/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/02/14 13:29:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "cpu.h"
#include <stdint.h>
#include <stdio.h>


 int load_program(const char *filename)
  {
   
      uint16_t count;
      uint8_t cur;
      FILE *file;
  
      //open file in binary read mode
      //read byte by byte into memory
      if(!(file = fopen(filename,"rb"))) {
          fprintf(stderr, "Error opening file %s\n", filename);
          return 0;
     }
  
      for (count = 0; count <= CART_HEADER_END; count++) {
      //Read file contents into buffer
        fread(&cur, 1, 1, file);
        printf("read:%d\n",cur);
        setMem(count, cur);
      }
      fclose(file);
      return 1;
  }


int main(int argc, char* argv[]) {

    if(argc < 2) {
        printf("usage ./gb game");
        return 1;
    }
    printf("ww\n");
    if( !load_program(argv[1])){
        printf("failed");
        return 1;
    }
    printf("done\n");
    
    uint16_t i;
    /*  print rom name */
    for(i = ROM_NAME_START; i<= ROM_NAME_END; i++) {
        printf("%c",getMem(i));
    }
    printf("\nfinished");
    
    /*  while (1) {
        printf("pc: %x\n opcode: %x\n",reg.PC,mem[reg.PC]);
        
        ins[mem[reg.PC++]]();
    }*/

}
