#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>


#include "matrix.h"


#define MAX_CMD_COUNT 50

/*protected functions*/
void load_matrix (Matrix_t* m, unsigned int* data);

/* 
 * PURPOSE: instantiates a new matrix with the passed name, rows, cols 
 * INPUTS: 
 *	name the name of the matrix limited to 50 characters 
 *  rows the number of rows the matrix
 *  cols the number of cols the matrix
 * RETURN:
 *  If no errors occurred during instantiation then true
 *  else false for an error in the process.
 *
 **/

bool create_matrix (Matrix_t** new_matrix, const char* name, const unsigned int rows,
						const unsigned int cols) {

	//TODO ERROR CHECK INCOMING PARAMETERS(finished)
	

	if(*new_matrix != NULL){
		printf("Already exist new matrix.\n");
		return false;
	}
	else if(!name){
		printf("No name while create matrix.\n");
		return false;
	}
	else if(rows<0 || cols<0){
		printf("Invalid numberof rows or colums.\n");
		return false;
	}


	*new_matrix = calloc(1,sizeof(Matrix_t));
	if (!(*new_matrix)) {
		return false;
	}
	(*new_matrix)->data = calloc(rows * cols,sizeof(unsigned int));
	if (!(*new_matrix)->data) {
		return false;
	}
	(*new_matrix)->rows = rows;
	(*new_matrix)->cols = cols;
	unsigned int len = strlen(name) + 1; 
	if (len > MATRIX_NAME_LEN) {
		return false;
	}
	strncpy((*new_matrix)->name,name,len);
	return true;

}

	//TODO FUNCTION COMMENT(finished)

/* 
 * PURPOSE: free the memory of given matrix
 * INPUTS: 
 * m: An address of a matrix that need to be free.
 * RETURN:
 * No return.
 *
 **/


void destroy_matrix (Matrix_t** m) {

	//TODO ERROR CHECK INCOMING PARAMETERS(finished)
	if(!m){
		printf("No matrix for destroy.\n");
		return;
	}	
	free((*m)->data);
	free(*m);
	*m = NULL;
}


	
	//TODO FUNCTION COMMENT(finished)

/* 
 * PURPOSE: Compare two matrix to see if they are same.
 * INPUTS: 
 *  a: Matrix need to compare with b
 *  b: Matrix need to compare with a
 * RETURN:
 *  If no errors and two matrices are same then true
 *  else false for an error in the process.
 *
 **/

bool equal_matrices (Matrix_t* a, Matrix_t* b) {

	//TODO ERROR CHECK INCOMING PARAMETERS(done below)
	
	if (!a || !b || !a->data || !b->data) {
		return false;	
	}

	int result = memcmp(a->data,b->data, sizeof(unsigned int) * a->rows * a->cols);
	if (result == 0) {
		return true;
	}
	return false;
}

	//TODO FUNCTION COMMENT(finished)

/* 
 * PURPOSE: Copy the matrix src into matrix dest
 * INPUTS: 
 *  src: the source matrix
 *  dest: the destination matrix
 * RETURN:
 *  If no errors occurred during the process of duplicate then true
 *  else false for an error in the process.
 *
 **/

bool duplicate_matrix (Matrix_t* src, Matrix_t* dest) {


	//TODO ERROR CHECK INCOMING PARAMETERS(finished)
	if (!src) {
		printf("There's no source matrix.\n");
		return false;
	}
	if(!dest){
		printf("There's no destination matrix.\n");
		return false;
	}
	/*
	 * copy over data
	 */
	unsigned int bytesToCopy = sizeof(unsigned int) * src->rows * src->cols;
	memcpy(dest->data,src->data, bytesToCopy);	
	return equal_matrices (src,dest);
}

	//TODO FUNCTION COMMENT(finished)
/* 
 * PURPOSE: shift the matrix to given direction for given bits
 * INPUTS: 
 *  a: matrix that need to be shift.
 *  direction: The direction of bit shift, either left or right.
 *  shift: number of bits that need to shift.
 * RETURN:
 *  If no errors occurred during the bit shift process then true
 *  else false for an error in the process.
 *
 **/

bool bitwise_shift_matrix (Matrix_t* a, char direction, unsigned int shift) {
	
	//TODO ERROR CHECK INCOMING PARAMETERS(finished)
	if (!a) {
		printf("Input matrix a is null!\n");
		return false;
	}
	if(direction != 'l'&&direction != 'r'){
		printf("Invalid input of direction!\n");
		return false;
	}
	if(shift < 0){
		printf("Invalid input of shift number!\n");
		return false;
	}

	if (direction == 'l') {
		unsigned int i = 0;
		for (; i < a->rows; ++i) {
			unsigned int j = 0;
			for (; j < a->cols; ++j) {
				a->data[i * a->cols + j] = a->data[i * a->cols + j] << shift;
			}
		}

	}
	else {
		unsigned int i = 0;
		for (; i < a->rows; ++i) {
			unsigned int j = 0;
			for (; j < a->cols; ++j) {
				a->data[i * a->cols + j] = a->data[i * a->cols + j] >> shift;
			}
		}
	}
	
	return true;
}

	//TODO FUNCTION COMMENT(finished)

/* 
 * PURPOSE: Add the contents of two matricies a and b together and store the 
 * result into a third matrix c.
 * INPUTS: 
 *  a: matrix that need to be added together with b.
 *  b: matrix that need to be added together with a.
 *  c: matrix that used to stord the result of a and b.
 * RETURN:
 *  If no errors occurred when added a and b together and store in c then true
 *  else false for an error in the process.
 *
 **/


bool add_matrices (Matrix_t* a, Matrix_t* b, Matrix_t* c) {

	//TODO ERROR CHECK INCOMING PARAMETERS(finished)
	if(!a || !b){
		printf("Invalid input of matrix! Cannot add.\n");
		return false;
	}
	if (a->rows != b->rows && a->cols != b->cols) {
		printf("Matrix a and b have different rows or columns, cannot add!\n");
		return false;
	}

	for (int i = 0; i < a->rows; ++i) {
		for (int j = 0; j < b->cols; ++j) {
			c->data[i * a->cols +j] = a->data[i * a->cols + j] + b->data[i * a->cols + j];
		}
	}
	return true;
}

	//TODO FUNCTION COMMENT(finished)

/* 
 * PURPOSE: Print out the contents of a given matrix.
 * INPUTS: 
 *  m: A matrix which contents need to be display.
 * RETURN:
 *  No return.
 *
 **/


void display_matrix (Matrix_t* m) {
	
	//TODO ERROR CHECK INCOMING PARAMETERS(finished)
	if(!m){
		printf("Input matrix is null, cannot display.\n");
		return;
	}

	printf("\nMatrix Contents (%s):\n", m->name);
	printf("DIM = (%u,%u)\n", m->rows, m->cols);
	for (int i = 0; i < m->rows; ++i) {
		for (int j = 0; j < m->cols; ++j) {
			printf("%u ", m->data[i * m->cols + j]);
		}
		printf("\n");
	}
	printf("\n");

}

	//TODO FUNCTION COMMENT(finished)

/* 
 * PURPOSE: Load a file with given name, use the content of the file to create a 
 * new matrix.
 * INPUTS: 
 *  matrix_input_filename: Name of the file which coontain the data of matrix need to create
 *  m: A matrix that created with input file's content.
 * RETURN:
 *  If a matrix with content of input file successfully created then true
 *  else false for an error in the process.
 *
 **/


bool read_matrix (const char* matrix_input_filename, Matrix_t** m) {
	
	//TODO ERROR CHECK INCOMING PARAMETERS(finished)
	if(!matrix_input_filename){
		printf("File name is null!\n");
		return false;
	}

	int fd = open(matrix_input_filename,O_RDONLY);
	if (fd < 0) {
		printf("FAILED TO OPEN FOR READING\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}

	/*read the wrote dimensions and name length*/
	unsigned int name_len = 0;
	unsigned int rows = 0;
	unsigned int cols = 0;
	
	if (read(fd,&name_len,sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READING FILE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}
	char name_buffer[50];
	if (read (fd,name_buffer,sizeof(char) * name_len) != sizeof(char) * name_len) {
		printf("FAILED TO READ MATRIX NAME\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;	
	}

	if (read (fd,&rows, sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READ MATRIX ROW SIZE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	if (read(fd,&cols,sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READ MATRIX COLUMN SIZE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	unsigned int numberOfDataBytes = rows * cols * sizeof(unsigned int);
	unsigned int *data = calloc(rows * cols, sizeof(unsigned int));
	if (read(fd,data,numberOfDataBytes) != numberOfDataBytes) {
		printf("FAILED TO READ MATRIX DATA\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;	
	}

	if (!create_matrix(m,name_buffer,rows,cols)) {
		return false;
	}

	load_matrix(*m,data);
	free(data);
	if (close(fd)) {
		return false;

	}
	return true;
}

	//TODO FUNCTION COMMENT(finished)

/* 
 * PURPOSE: Write the content of given matrix to a file with given name. 
 * INPUTS: 
 *  matrix_output_filename: Name of the file which will fill out by content of given matrix
 *  m: A matrix which content need to write to a file.
 * RETURN:
 *  If the matrix's content successfully write to a file then true.
 *  else false for an error in the process.
 *
 **/


bool write_matrix (const char* matrix_output_filename, Matrix_t* m) {
	
	//TODO ERROR CHECK INCOMING PARAMETERS(finished)
	if(!matrix_output_filename){
		printf("File name is null!\n");
		return false;
	}
	if(!m){
		printf("Input matrix is null!\n");
		return false;
	}

	int fd = open (matrix_output_filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	/* ERROR HANDLING USING errorno*/
	if (fd < 0) {
		printf("FAILED TO CREATE/OPEN FILE FOR WRITING\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXISTS\n");
		}
		return false;
	}
	/* Calculate the needed buffer for our matrix */
	unsigned int name_len = strlen(m->name) + 1;
	unsigned int numberOfBytes = sizeof(unsigned int) + (sizeof(unsigned int)  * 2) + name_len + sizeof(unsigned int) * m->rows * m->cols + 1;
	/* Allocate the output_buffer in bytes
	 * IMPORTANT TO UNDERSTAND THIS WAY OF MOVING MEMORY
	 */
	unsigned char* output_buffer = calloc(numberOfBytes,sizeof(unsigned char));
	unsigned int offset = 0;
	memcpy(&output_buffer[offset], &name_len, sizeof(unsigned int)); // IMPORTANT C FUNCTION TO KNOW
	offset += sizeof(unsigned int);	
	memcpy(&output_buffer[offset], m->name,name_len);
	offset += name_len;
	memcpy(&output_buffer[offset],&m->rows,sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy(&output_buffer[offset],&m->cols,sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy (&output_buffer[offset],m->data,m->rows * m->cols * sizeof(unsigned int));
	offset += (m->rows * m->cols * sizeof(unsigned int));
	output_buffer[numberOfBytes - 1] = EOF;

	if (write(fd,output_buffer,numberOfBytes) != numberOfBytes) {
		printf("FAILED TO WRITE MATRIX TO FILE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}
	
	if (close(fd)) {
		return false;
	}
	free(output_buffer);

	return true;
}

	//TODO FUNCTION COMMENT(finished)

/* 
 * PURPOSE: Give a matrix m, make the content of m be random number with certain range.
 * INPUTS: 
 *  m: A matrix which content need to be randomized
 *  start_range: Smallest number can be generated.
 *  end_range: Biggest number can be generated.
 * RETURN:
 *  If successfully fill out a matrix with random content with certain range then ture.
 *  else false for an error in the process.
 *
 **/


bool random_matrix(Matrix_t* m, unsigned int start_range, unsigned int end_range) {
	
	//TODO ERROR CHECK INCOMING PARAMETERS(finished)
	if(!m){
		printf("Input matrix is null!\n");
		return false;
	}
	if(!start_range||!end_range){
		printf("Either start or end range is null!\n");
		return false;
	}
	if(start_range > end_range){
		printf("start range is bigger than end range!\n");
		return false;
	}	


	for (unsigned int i = 0; i < m->rows; ++i) {
		for (unsigned int j = 0; j < m->cols; ++j) {
			m->data[i * m->cols + j] = rand() % (end_range + 1 - start_range) + start_range;
		}
	}
	return true;
}

/*Protected Functions in C*/

	//TODO FUNCTION COMMENT(finished)

/* 
 * PURPOSE: Load data from an array to a matrix m by memory copy.
 * INPUTS: 
 *  m: A matrix that will used to load data.
 *  data: An array whos content will used to fill out a matrix.
 *	(Or a pointer points to data)
 * RETURN:
 *  No return.
 **/


void load_matrix (Matrix_t* m, unsigned int* data) {
	
	//TODO ERROR CHECK INCOMING PARAMETERS(finished)
	if(!m){
		printf("Input matrix is null, no place to hold data!\n");
		return;
	}
	if(!data){
		printf("Data is null!\n");
		return;
	}

	memcpy(m->data,data,m->rows * m->cols * sizeof(unsigned int));
}

	//TODO FUNCTION COMMENT(finished)

/* 
 * PURPOSE: Add the given matrix to an array of matrix and if there's already exit an matrix then
 * destroy the old matrix and replace with new matrix.
 * INPUTS: 
 *  mats: An array of matrices
 *  new_matrix: A matrix that need to add to array list.
 *  num_mats: Number of matricies.
 * RETURN:
 *  A position number of the newly added matrix.
 *
 **/


unsigned int add_matrix_to_array (Matrix_t** mats, Matrix_t* new_matrix, unsigned int num_mats) {
	
	//TODO ERROR CHECK INCOMING PARAMETERS(finished)
	if(!mats){
		printf("Invalid input mats\n");
		return -1;
	}
	else if(!new_matrix){
		printf("No matrix can add to list!\n");
		return -1;
	}
	else if(!num_mats){	
		printf("Number of matrix is null!\n");
		return -1;
	}

	static long int current_position = 0;
	const long int pos = current_position % num_mats;
	if ( mats[pos] ) {
		destroy_matrix(&mats[pos]);
	} 
	mats[pos] = new_matrix;
	current_position++;
	return pos;
}
