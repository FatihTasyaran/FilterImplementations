//#include <iostream>
#include<cmath>
//#include "bf_3.hpp"
#include "mlbf.hpp"
#include <omp.h>

int main(){
  /*
  BloomFilter_OT* bf = new BloomFilter_OT(800000, 16, 0); //bit size, no_hash functions, bf_id
  bf->insert(23, 42);
  bf->insert(46, 23);
  bf->insert(79, 22);

  //std::cout << bf->query(46,23) << std::endl;
  //std::cout << bf->query(46,24) << std::endl;

  //bf->single_insert(8);
  //bf->single_insert(13446);
  //bf->single_insert(783992);
  //bf->single_insert(1348);
  //bf->single_insert(22);


  //std::cout << bf->single_query(24) << std::endl;
  //std::cout << bf->single_query(2409123) << std::endl;
  //std::cout << bf->single_query(783992) << std::endl;
  
  
  //GENERATING A SET
  
  int insert[100000];
  int query[100000];
  
  for(int i = 0; i < 100000; i++){
    insert[i] = i;
    query[i] = 100000 + i;
  }

  double start = omp_get_wtime();
  for(int i = 0; i < 100000; i++){
    bf->single_insert(insert[i]);
  }
  double duration = omp_get_wtime() - start;

  std::cout << "Insert Speed: " << 100000 / duration << " integers/s" << std::endl;
  
  double fp = 0;

  double start2 = omp_get_wtime();
  for(int i = 0; i < 100000; i++){
    if(bf->single_query(query[i]))
      fp += 1;
  }
  double duration2 = omp_get_wtime() - start2;

  std::cout << "Query Speed: " << 100000 / duration2 << " integers/s" << std::endl;

  std::cout << "False Positive: " << fp/100000 << std::endl;
  std::cout << "hur.st reported for same parameters: 0.097" << std::endl;

  */
  std::cout << "Creating" << std::endl;

  BloomFilter_MT* mlbf = new BloomFilter_MT(1000, 1, 128, 4);

  std::cout << "Created" << std::endl;

  /*
  mlbf->insert(43,0);
  mlbf->insert(43,1);
  mlbf->insert(43,2);
  mlbf->insert(43,3);
  mlbf->insert(43,4);
  mlbf->insert(43,5);
  mlbf->insert(43,6);
  
  std::cout << mlbf->query(1,3) << std::endl;
  std::cout << mlbf->query(2,3) << std::endl;
  std::cout << mlbf->query(3,3) << std::endl;
  std::cout << mlbf->query(4,3) << std::endl;
  std::cout << mlbf->query(5,3) << std::endl;
  std::cout << mlbf->query(6,3) << std::endl;
  std::cout << mlbf->query(7,3) << std::endl;
  */

  mlbf->insert(1,0);

  
  for(int i = 0; i < 100; i++){
    std::cout << "i: " << i << std::endl;
    mlbf->insert(i, 0);
  }

  for(int i = 100; i < 200; i++){
    std::cout << mlbf->query(i,0) << std::endl;
  }
  
    
  return 0;
  
}
