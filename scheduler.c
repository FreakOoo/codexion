#include "codexion.h

t_ask_forstick create_request(t_person *coder, t_stick *usb)
{
  //check time to burnout or number in line
  //give them a yes or no depending on if he's allowed to 
  //bind with the stick or not.
  //
  t_ask_forstick the_ask;

  the_ask.coder = coder->name;
  the_ask.permission = stick->next_inline;
  stick->next_inline++;

  if(coder->config->scheduler == FIFO)
    the_ask.permission = stick->next_inline;

  else
    the_ask.key = coder->last_compile + coder->config->time_to_burnout;
  return(the_ask)
}

