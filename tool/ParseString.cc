#include "../include/ParseString.h"

int getWordEnd(char * s,int start,int end)
{
      while(start < end && s[start] != SPACE && s[start] != TABLE && s[start] != END)
      {
            ++start;
      }
      return start;
}