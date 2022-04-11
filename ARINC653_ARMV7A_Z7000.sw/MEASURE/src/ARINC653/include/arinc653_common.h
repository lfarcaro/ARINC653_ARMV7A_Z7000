#ifndef ARINC653_COMMON_H_
#define ARINC653_COMMON_H_

// Null-terminated string copy method
BOOLEAN_TYPE COMMON_COPYSTRING(CHARACTER_TYPE *FROM, CHARACTER_TYPE *TO, SIZE_TYPE MAXIMUM_LENGTH);

// Null-terminated string comparison method
BOOLEAN_TYPE COMMON_COMPARESTRINGS(CHARACTER_TYPE *STRING1, CHARACTER_TYPE *STRING2, SIZE_TYPE MAXIMUM_LENGTH);

#endif
