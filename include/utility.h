#ifndef _UTILITY_H
#define _UTILITY_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&); \
    void operator=(const TypeName&) = delete

#endif // _UTILITY_H