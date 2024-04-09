// CMSC 341 - Spring 2021 - Project 4
// hash.cpp
// The impleme ntation of the class HashTable
#include "hash.h"
#include "math.h"
HashTable::HashTable(unsigned size, hash_fn hash){

  if(size  < MINPRIME){// The conditonal for chossing the capacity for the table
    m_capacity = MINPRIME;
  }
  else if (size > MAXPRIME){
    m_capacity = MAXPRIME;
  }
  else{
    if(isPrime(size) == true){// Check to see if the number chosen if prime and if it is make it the capacity
      m_capacity = size;
    }
    else{
      m_capacity = findNextPrime(size);// If its not then choose the next closest prime muner
    }
  }

  m_table= new Person[m_capacity];// Make the table with the given capacity
  int i = 0;
  for(i = 0; i < m_capacity; i++){// Fill the entire thing with the empty object
    m_table[i] = EMPTY;
  }

  m_size = 0;
  m_numDeleted = 0;
  m_hash = hash;
  m_currentProb = LINEAR;

}

HashTable::~HashTable(){

  m_capacity = 0;// Set evetyhing to 0 and delete the table 
    m_size = 0;    
    m_numDeleted = 0;
    m_hash = nullptr;
    delete []m_table;       

}

bool HashTable::insert(Person person){
  int a = 0;
  for(a = 0; a < m_capacity; a++){
  
    if(person.serial() == m_table[a].serial() && person.key() == m_table[a].key() ){// If there is dulcate than return flase
      return false;
    }
  }
  
  int number = m_hash(person.key())% m_capacity;

  if(m_table[number].key() == ""){// If the place is empty put it in that space
    m_table[number] = person;
  }
  
  else{
    bool inputted = false;
    int i = 0;
    if(m_currentProb == LINEAR){ // If its linear than use the earn forumal
      while(inputted != true){// Goees until it finds a place for it
     
      int index = (m_hash(person.key()) + i) % m_capacity;
      if(m_table[index].key() == ""){// Than add it to it splace
	m_table[index] = person;
	inputted = true;
      }
      i++;

    }
    }
    else{
      while(inputted != true){// This is the quadratric fimaula version same thing but with the quadratic formula

	int index = ((m_hash(person.key()) % m_capacity) + (i * i)) % m_capacity;
	if(m_table[index].key() == ""){
	  m_table[index] = person;
	  inputted = true;
	}
	i++;
	
      }
    }

  }

  m_size ++;

  if(lambda() > .5){// If lamabad if greter than .5 than change the current Prob based what the current one is 
    if(m_currentProb == LINEAR){
      setProbingPolicy(QUADRATIC);
    }
    else{
      setProbingPolicy(LINEAR);
    }
  }

  return true;
}

bool HashTable::remove(Person person){
  int i = 0;
 
  int index  = m_hash(person.key())% m_capacity;
  if(m_table[index].key() == person.key()){// If the index is found than deleted it
    m_table[index] = DELETED;
    m_numDeleted ++;
 
  
    if(deletedRatio() > .25){// Check if the ration is greater than .25 than change prob function
      if(m_currentProb == LINEAR){
	setProbingPolicy(QUADRATIC);
      }
    
      else{
	setProbingPolicy(LINEAR);
      }
  }    
  

  return true;
  }
  else{// If it was found using the hash function than check with the probing eqaution
    int i = 0;
    bool inputted = false;
    if(m_currentProb == LINEAR){
      while(inputted != true && i < m_capacity){// until the thing is found or the capoacity is reached

      int index = (m_hash(person.key()) + i) % m_capacity;
      if(m_table[index].key() == person.key()){// If the thing is found than deleted it
	m_table[index] = DELETED;
	m_numDeleted ++;

        inputted = true;

      }
      i++;

    }
      if(inputted == true){// If true cehck to see if the ratio meet the condtion if it is than change based on current prob
	if(deletedRatio() > .25){
	  if(m_currentProb == LINEAR){
	    setProbingPolicy(QUADRATIC);
	  }
	  
	  else{
	    setProbingPolicy(LINEAR);
	  }
	
	}
	return true;

      }
      else{
	return false;
      }

    }
    else{
      while(inputted != true && i <m_capacity){// This is for the quadratic formal calacution to tfind the spot

        int index = ((m_hash(person.key()) % m_capacity) + (i * i)) % m_capacity;
        if(m_table[index].key() ==  person.key()){
	  m_table[index] = DELETED;
	  m_numDeleted ++;


          inputted = true;
        }
        i++;

      }
      if(inputted == true){// If something was removed than check to see if the deleted ration is more than .25 and if it is cahnge forumal
        if(deletedRatio() > .25){
          if(m_currentProb == LINEAR){
            setProbingPolicy(QUADRATIC);
          }

          else{
            setProbingPolicy(LINEAR);
          }
       
	}
	return true;
      }
      else{
        return false;
      }

    }


  }
}

void HashTable::setProbingPolicy(probing prob){
  m_currentProb = prob;

  m_size  = m_size - m_numDeleted;// datapoints

  int newcapacity = findNextPrime(m_size*4);// The new capacity
  Person *new_table = m_table;// Temp for old table


  int temp = m_capacity;
  m_capacity = newcapacity;

  m_table= new Person[m_capacity];// new tbale with the new capcacity
  
 
  m_size = 0;
  int a = 0;
  for(a = 0; a < temp ; a++){// Goes through a loop and only at the thing that is sdeleted
    if(new_table[a].key() != DELETEDKEY){
      insert(new_table[a]);
    }
    
  }
  delete []new_table;// delete the old and useless table
  m_numDeleted = 0;// Turn numdeleted back to 0 since no delted exist now
}

float HashTable::lambda() const {

  // use flaot becuase if i don't it will return an int but that wull be zero
  return float(m_size)/float(m_capacity);
}

float HashTable::deletedRatio() const {
  // This return a decimal that is less than 1 and won't beomce 0
  return float(m_numDeleted)/float(m_size);
}

void HashTable::dump() const {
  for (int i = 0; i < m_capacity; i++) {
    cout << "[" << i << "] : " << m_table[i] << endl;
  }
}

bool HashTable::isPrime(int number){
    // If number is prime this function returns true
    // otherwise it returns false
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

int HashTable::findNextPrime(int current){
    //we won't go beyond MAXPRIME
    //the smallest prime would be next after MINPRIME
    if (current < MINPRIME) current = MINPRIME;
    for (int i=current; i<MAXPRIME; i++) { 
        for (int j=2; j*j<=i; j++) {
            if (i % j == 0) 
                break;
            else if (j+1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    //if a user tries to go over MAXPRIME
    return MAXPRIME;
}
