#include "bf_3.hpp"
#include <stdio.h>
#include <math.h>


struct BloomFilter_MT{
private:
  BloomFilter_OT** filters;
  int size_kb; //in KBs
  int granularity; // 0->Coarse-Grain 1->Medium-Grain 2->Fine-Grain
  int no_partitions;
  int no_layers;
  int no_hashes; // This apply for every filter in structure
  int no_parts_in_leaf_filter;
  

public:
  BloomFilter_MT(int SIZE_KB, int GRANULARITY, int NO_PARTITIONS, int NO_HASHES) : size_kb(SIZE_KB), granularity(GRANULARITY), no_partitions(NO_PARTITIONS), no_hashes(NO_HASHES){

    //int no_parts_in_leaf_filter;
    int granul; // This is to calculate number of layers given granularity

    if(granularity == 0){

      if(no_partitions <= 512){
	no_parts_in_leaf_filter = no_partitions / 4;
	granul = 4;
      }
      else{
	no_parts_in_leaf_filter = no_partitions / 8;
	granul = 8;
      }
    }

    else if(granularity == 1){

      if(no_partitions <= 512){
	no_parts_in_leaf_filter = no_partitions / 8;
	granul = 8;
      }
      else{
	no_parts_in_leaf_filter = no_partitions / 16;
	granul = 16;
      }
    }
    
    else if(granularity == 2){
      no_parts_in_leaf_filter = 1;
      granul = no_partitions;
    }

    no_layers = log2(granul) + 1;
    int layer_size = ceil(size_kb/no_layers);
    
    filters = new BloomFilter_OT*[no_layers];
    
    int no_filters_in_layer = no_partitions / no_parts_in_leaf_filter;
    int filter_id = 0;

        
    for(int i = no_layers - 1; i >= 0; i--){
      filters[i] = new BloomFilter_OT[no_filters_in_layer];
      
      for(int j = 0; j < no_filters_in_layer; j++){
	std::cout << "Creating filter " << filter_id << " Layer: " << i << " Filter Size: " << (size_kb*8000)/no_filters_in_layer/no_layers << " No filters in layer: " << no_filters_in_layer <<std::endl;
	filters[i][j] = BloomFilter_OT((((size_kb*8000)/no_filters_in_layer)/no_layers), no_hashes, filter_id);
	filter_id++;
      }
      no_filters_in_layer /= 2;
    }
  }

  bool query(int edge, int part){
    int layer = no_layers - 1; 
    int j;

    if(part != 0){
      j = no_parts_in_leaf_filter / part;
    }
    else{
      j = 0;
    }
    
    return filters[layer][j].single_query(edge);
  }

  /* Layer query queries a specific layer of the structure. If value is found within given layer it aborts inserting to higher layers 
   */
  bool layer_query(int edge, int part, int layer){
    int j = part / ((no_partitions / no_parts_in_leaf_filter) / pow(2, (no_layers-1) - layer));
    return filters[layer][j].single_query(edge);
  }
  
  void insert(int edge, int part){
    
    int layer = no_layers - 1;
    int j;
    
    if(part != 0)
      j = no_parts_in_leaf_filter / part;
    else
      j = 0;
    
    for(int current_layer = layer; current_layer >= 0; current_layer--){
           
      if(layer_query(edge, part, current_layer)){
	std::cout << "Layer query returned:  " << layer_query(edge, part, current_layer) << std::endl;
	break;
      }
      else{
	filters[current_layer][j].print_id();
	std::cout << "Before call current layer: " << current_layer << " j: " << j << std::endl;
	filters[current_layer][j].single_insert(edge);
      }
           
      j = edge / ((no_partitions / no_parts_in_leaf_filter) / pow(2, layer - current_layer));
      
    }
  }
  
};
    
	
    
    
    
    
  
