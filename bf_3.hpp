#include<iostream>
#include<bitset>
#include<cstring>
#include<cmath>
#include<stdint.h>


bool isPrime(int n)  
{  

  if (n <= 1)  return false;  
  if (n <= 3)  return true;  

  if (n%2 == 0 || n%3 == 0) return false;  
    
  for (int i=5; i*i<=n; i=i+6)  
    if (n%i == 0 || n%(i+2) == 0)  
      return false;  
    
  return true;  
}  
  
int nextPrime(int N) 
{ 
  if (N <= 1) 
    return 2; 
  
  int prime = N; 
  bool found = false; 
  
  while (!found) { 
    prime++; 
  
    if (isPrime(prime)) 
      found = true; 
  } 
  
  return prime; 
} 

struct OTax_b_hash{
private:
  int a;
  int b;
  int p;
  int size;

public:
  
  OTax_b_hash()
  {}
  
  OTax_b_hash(int num, int bf_id, int csize, int hash_count){
    a = 71212893 + bf_id + num*2313322;
    b = 190934441 + bf_id + num*124013843;

    size = csize;
    
    for(int i = 0; i < hash_count; i++){
      if(num == i){
	//p = nextPrime(ceil(2*size/(i+3)));
	p = nextPrime(ceil(2*size/(i+2)));
      }
    }
    
    if(num == hash_count-1){
      this->p = nextPrime(csize*0.85);
      while(this->p < csize-(csize*0.05))
	{
	  this->p = nextPrime(this->p);
	}
    }
  }
  
  int OThash(int val1, int val2){

    int pre_sum_val = (val1 * val2);
    int hash_val = (a*pre_sum_val + b)%p;
  
    if(hash_val < 0){
      hash_val *= -1;
      hash_val = hash_val%p;
    }
    
    return hash_val;
  }  

  int single_OThash(int val1){

    //int pre_sum_val = (val1 * val2);
    //std::cout << "a, b, p, val: " << a << " " << b << " " << p << " " << val1 << std::endl;
    std::cout << "a: " << a << std::endl;
    std::cout << "b: "<< b << std::endl;
    std::cout << "p: " << p << std::endl;
    std::cout << "val1: " << val1 << std::endl;
    
    int hash_val = (a*val1 + b)%p;
  
    if(hash_val < 0){
      hash_val *= -1;
      hash_val = hash_val%p;
    }
    
    return hash_val;
  }

  void print_info(){
    std::cout << "//HASH INFO//" << std::endl;
    std::cout << "//a: " << a << "//\n";
    std::cout << "//b: " << b << "//\n";
    std::cout << "//p: " << p << "//\n";
    std::cout << "//size: " << size << "//\n"; 
  }
};


void set_bit(uint32_t* &ints, int big_loc, int small_loc){
  ints[big_loc] |= (1U << (small_loc-1));
}

void clear_bit(uint32_t* &ints, int big_loc, int small_loc){
  ints[big_loc] &= ~(1U << (small_loc-1));
}

bool check_bit(uint32_t* &ints, int big_loc, int small_loc){
  return (ints[big_loc] & (1 << (small_loc - 1)));
}

void toggle_bit(uint32_t* &ints, int big_loc, int small_loc){
  if(!check_bit(ints, big_loc, small_loc))
    ints[big_loc] ^= (1U << (small_loc-1));
}


struct BloomFilter_OT {
private:
  uint32_t* ints;
  uint32_t no_bits;
  int bf_id;
  int hash_count;
  uint32_t no_ints;
  //std::bitset<NOBITS> bits;
  OTax_b_hash** hashes;//[NOHASHES];
  
public:
  int hash(int val1, int val2, int k) {
    int index = hashes[k]->OThash(val1, val2);
    return index;
  }
  
  void insert(int val1, int val2){
    for(int k = 0; k < hash_count; k++){
      int index = hash(val1, val2, k);
      int big_loc = index/32;
      int small_loc = index - (big_loc*32);
      set_bit(ints, big_loc, small_loc);
    }
  }
  
  bool query(int val1, int val2){
    for(int k = 0; k < hash_count; k++){
      int index = hash(val1, val2, k);
      int big_loc = index/32;
      int small_loc = index - (big_loc*32);
      check_bit(ints, big_loc, small_loc);
      if(!check_bit(ints, big_loc, small_loc))
	return false;
    }
    return true;
  }

  int single_hash(int val1, int k){
    std::cout << "single hash k: " << k << std::endl;
    hashes[k]->print_info();
    int index = hashes[k]->single_OThash(val1);
    return index;
  }

  void single_insert(int val1){
    std::cout << "hash count: " << this->hash_count << std::endl;
    for(int k = 0; k < this->hash_count; k++){
      std::cout << "Loop k: " << k << std::endl;
      int index = single_hash(val1, k);
      int big_loc = index/32;
      int small_loc = index - (big_loc*32);
      set_bit(ints, big_loc, small_loc);
    }
  }

  bool single_query(int val1){
    for(int k = 0; k < hash_count; k++){
      int index = single_hash(val1, k);
      int big_loc = index/32;
      int small_loc = index - (big_loc*32);
      check_bit(ints, big_loc, small_loc);
      if(!check_bit(ints, big_loc, small_loc))
	return false;
    }
    return true;
  }
  
  BloomFilter_OT(int no_bits, int hashCount, int bf_id) : no_bits(no_bits), bf_id(bf_id), hash_count(hashCount){
    no_ints = no_bits/32;
    //hash_count = hashCount;
    ints = new uint32_t[no_ints];
    memset(ints, 0, sizeof(uint32_t) * no_ints);
    
    hashes = new OTax_b_hash*[hashCount];
    for(int i = 0; i < hashCount; i++){
      hashes[i] = new OTax_b_hash(i, bf_id, no_bits, hashCount);
    }
  }

  void print_id(){
    std::cout << "Bloom Filter with id #" << this->bf_id << "#" << std::endl;
  }
  
  BloomFilter_OT()
  {}
  
  ~BloomFilter_OT() {
    //delete [] ints;
    //delete [] hashes;
  }
};
