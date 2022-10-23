#include "../include/ParseString.h"

void locateWordIndex(char * s,int &start,int &end)
{
      while(start < end && (s[start] == END || s[start] == SPACE || s[start] == TABLE))
      {
            ++start;
      }
      int temp = start + 1;
      while(temp < end && s[start] != SPACE && s[start] != TABLE && s[start] != END)
      {
            ++temp;
      }
      end = temp;
}