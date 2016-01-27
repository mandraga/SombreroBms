
#include <stdio.h>

#include "keypress.h"

Ckeypress::Ckeypress()
{
  int i;

  // Keypressed events
  for (i = 0; i < (int)(sizeof(m_bkeypress) / sizeof(bool)); i++)
    {
      m_bkeypress[i] = false;
    }
}

void Ckeypress::key_on(int code)
{	
  m_bkeypress[code % 256] = true;
//  printf("%c on\n", code);
}

void Ckeypress::key_off(int code)
{
  m_bkeypress[code % 256] = false;
//  printf("%c off\n", code);
}

bool Ckeypress::is_pressed(int code)
{
  return (m_bkeypress[code % 256]);
}
