//  Copyright 2013 Google Inc. All Rights Reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define max_size  2000         // max length of strings
#define max_doc_size  256      // max count of words in doc
#define N  40                 // number of closest words that will be shown
#define max_w  50              // max length of vocabulary entries
#define max_question_len 10000
long long words, size;
float *M;
char *vocab;

void str_to_vec(char st1[], float *vec, long long *bi);
float distance(float *vec1, float *vec2);
void str_to_words_vec(char st1[], long long *bi) {
	long long a, b, c,d, cn;
	char st[256][max_size];
	float len;
	//  printf("string: %s\n", st1);
	cn = 0;
	b = 0;
	c = 0;
	while (st1[c] == ' ')
		c++;
	while (1) {
		st[cn][b] = st1[c];
		b++;
		c++;
		st[cn][b] = 0;
		if (st1[c] == 0) break;
		if (st1[c] == ' ') {
			cn++;
			b = 0;
			c++;
		}
	}
	cn++;
	d = 0;
	for (a = 0; a < max_doc_size; a++) bi[a] = 0;
	for (a = 0; a < cn; a++) {
		for (b = 0; b < words; b++) 
			if (!strcmp(&vocab[b * max_w], st[a])) break;
		if (b == words)
		continue;
		
		bi[d] = b;
		d++;
		//    printf("\nWord: %s  Position in vocabulary: %lld\n", st[a], bi[a]);
		//if (b == -1) {
			//      printf("Out of dictionary word!\n");
			//break;
		//}
	}
	bi[d] = -1;
	//  if (b == -1) return;
	//  printf("\n                                              Word       Cosine distance\n------------------------------------------------------------------------\n");
	//for (a = 0; a < size; a++) vec[a] = 0;
//	for (b = 0; b < cn; b++) {
	//	if (bi[b] == -1) return;
	//	for (a = 0; a < size; a++) vec[a] += M[a + bi[b] * size];
	//}

	//len = 0;
	//for (a = 0; a < size; a++) len += vec[a] * vec[a];
	//len = sqrt(len);
	//for (a = 0; a < size; a++) vec[a] /= len;

	return;
}
void str_to_vec(char st1[], float *vec, long long *bi) {
  long long a, b, c, cn;
  char st[256][max_size];
  float len;
//  printf("string: %s\n", st1);
  cn = 0;
  b = 0;
  c = 0;
  while (1) {
    st[cn][b] = st1[c];
    b++;
    c++;
    st[cn][b] = 0;
    if (st1[c] == 0) break;
    if (st1[c] == ' ') {
      cn++;
      b = 0;
      c++;
    }
  }
  cn++;
  
  for (a = 0; a < max_doc_size; a++) bi[a] = 0;
  for (a = 0; a < cn; a++) {
    for (b = 0; b < words; b++) if (!strcmp(&vocab[b * max_w], st[a])) break;
    if (b == words) b = -1;
    bi[a] = b;
//    printf("\nWord: %s  Position in vocabulary: %lld\n", st[a], bi[a]);
    if (b == -1) {
//      printf("Out of dictionary word!\n");
      break;
    }
  }
  
//  if (b == -1) return;
//  printf("\n                                              Word       Cosine distance\n------------------------------------------------------------------------\n");
  for (a = 0; a < size; a++) vec[a] = 0;
  for (b = 0; b < cn; b++) {
    if (bi[b] == -1) return;
    for (a = 0; a < size; a++) vec[a] += M[a + bi[b] * size];
  }
  
  len = 0;
  for (a = 0; a < size; a++) len += vec[a] * vec[a];
  len = sqrt(len);
  for (a = 0; a < size; a++) vec[a] /= len;
  
  return;
}

//float distance(float *vec1, float *vec2) {
//  float dist;
//  
//  dist = 0;
//  for (int a = 0; a < size; a++) dist += vec1[a] * vec2[a];
//  
//  return dist;
//}

float distance(float *A, float *B) {
  double dot = 0.0, denom_a = 0.0, denom_b = 0.0 ;
  for(unsigned int i = 0u; i < size; ++i) {
    dot += A[i] * B[i] ;
    denom_a += A[i] * A[i] ;
    denom_b += B[i] * B[i] ;
  }
  return dot / (sqrt(denom_a) * sqrt(denom_b)) ;
}

const char* getfield(char* line, int num) {
  const char* token;
  char s[2] = ",";
  token = strtok(line, s);
  
  int i = 0;
  while( token != NULL ) {
    if (i == num) {
      return token;
    }
    token = strtok(NULL, s);
    i++;
  }
  
  return NULL;
}



int main(int argc, char **argv) {
  FILE *f;
  char *bestw[N];
  float temp, dist, len, bestd[N], vec[max_size], ans[max_size];
  long long a, b, c, cn, bi[max_doc_size], abi[max_doc_size];
  int first_ans_i;

  f = fopen("E:\\GitDir\\Similarity\\cosi101a\\vectors.bin.big", "rb");
  if (f == NULL) {
    printf("Input file not found\n");
    return -1;
  }
  fscanf(f, "%lld", &words);
  fscanf(f, "%lld", &size);
  vocab = (char *)malloc((long long)words * max_w * sizeof(char));
  for (a = 0; a < N; a++) bestw[a] = (char *)malloc(max_size * sizeof(char));
  M = (float *)malloc((long long)words * (long long)size * sizeof(float));
  if (M == NULL) {
    printf("Cannot allocate memory: %lld MB    %lld  %lld\n", (long long)words * size * sizeof(float) / 1048576, words, size);
    return -1;
  }
  for (b = 0; b < words; b++) {
    a = 0;
    while (1) {
      vocab[b * max_w + a] = fgetc(f);
      if (feof(f) || (vocab[b * max_w + a] == ' ')) break;
      if ((a < max_w) && (vocab[b * max_w + a] != '\n')) a++;
    }
    vocab[b * max_w + a] = 0;
    for (a = 0; a < size; a++) fread(&M[a + b * size], sizeof(float), 1, f);
    len = 0;
    for (a = 0; a < size; a++) len += M[a + b * size] * M[a + b * size];
    len = sqrt(len);
    for (a = 0; a < size; a++) M[a + b * size] /= len;
  }
  fclose(f);
  for (a = 0; a < N; a++) bestd[a] = 0;
  for (a = 0; a < N; a++) bestw[a][0] = 0;

  for (int i = 0; i < argc; ++i) {
    printf("argv[%d]: %s\n", i, argv[i]);
  }
  
  FILE* stream;
//  stream = fopen("/Users/joshsilverman/Dropbox/Apps/cosi101a/cosi101a/data/validation_set.csv", "r");
//  first_ans_i = 2;
  
  stream = fopen("E:\\GitDir\\Similarity\\cosi101a\\data\\validation_set.csv", "r");
  first_ans_i = 2;
  
  char line[max_question_len];
  
  fgets(line, max_question_len, stream);
  while (fgets(line, max_question_len, stream)) {
    
    size_t len = strlen(line);
    if (len > 0 && line[len-1] == '\n') {
      line[--len] = '\0';
    }
    
    char* tmp = strdup(line);
    char* field = getfield(tmp, 1);//question
//    printf("\nQuestion: %s\n", field);
    str_to_words_vec(field, bi);

    tmp = strdup(line);
    field = getfield(tmp, 0);
	//float temp = 0;
	//int num = 0;
    printf("\n%s", field);//question number
	int q = 0;
	int p = 0;
    for (int i = first_ans_i; i <= first_ans_i + 3; i++) {
      tmp = strdup(line);
      field = getfield(tmp, i);//answers
//      printf("%i: %s,", i, field);      
      str_to_words_vec(field, abi);
	  q = 0;	  
	  dist = 0;
	  len = 0;
	 //  temp = 0;
	  // num = 0;
	  /*
	   len = 0;
  for (a = 0; a < size; a++) len += vec[a] * vec[a];
  len = sqrt(len);
  for (a = 0; a < size; a++) vec[a] /= len;*/
	  while (bi[q] != -1)
	  {
		
		  for (int a = 0; a < size; a++) vec[a] = M[a + bi[q] * size];
		  while (abi[p] != -1)
		  {
			  
			  for (int a = 0; a < size; a++) ans[a] = M[a + abi[p] * size];
			  
			  temp= distance(vec, ans);
			  if (temp > 0.2)
			  {
				  dist += temp;
				  len++;
			  }
			  p++;
		  }
		  //len += p;
		  p = 0;
		  q++;
	  }
	  dist /= len;
      printf(", %f", dist);
    }

    free(tmp);
  }
}
