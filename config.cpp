#include "config.h"

bool verify_user(String user_id){

  for (int i = 0; i < (int) (sizeof(accepted_users) / sizeof(accepted_users[0])); i++){
    if (user_id == accepted_users[i]){
      return true;
    }
  }

  return false;
}
