#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct TCriminal
{
    struct TCriminal  * m_Next;
    char              * m_Name;
    struct TCriminal ** m_Contacts;
    size_t              m_Cnt;
    size_t              m_Capacity;
} TCRIMINAL;
#define nullptr NULL
#endif /* __PROGTEST__ */

TCRIMINAL        * createRecord ( const char      * name,
                                  TCRIMINAL       * next )
{
    TCRIMINAL * new_node = (TCRIMINAL*)calloc(1,sizeof(TCRIMINAL));
    new_node->m_Next = next;
    new_node->m_Name = (char*)calloc(strlen(name) + 1, sizeof(char));
    new_node->m_Cnt = 0;
    new_node->m_Capacity = 2;
    new_node->m_Contacts = (TCRIMINAL**)calloc(new_node->m_Capacity, sizeof (TCRIMINAL*));
    new_node->m_Name = strcpy(new_node->m_Name, name);
    return new_node;
}

TCRIMINAL * copy_of_node( TCRIMINAL* src){
    TCRIMINAL* new_node = (TCRIMINAL*)calloc(1,sizeof(TCRIMINAL));
    new_node->m_Next = src->m_Next;
    new_node->m_Cnt = src->m_Cnt;
    new_node->m_Capacity = src->m_Capacity;
    new_node->m_Name = (char*)calloc(strlen(src->m_Name) + 1, sizeof(char));
    new_node->m_Name = strcpy(new_node->m_Name, src->m_Name);
    new_node->m_Contacts = (TCRIMINAL**)calloc(new_node->m_Capacity, sizeof(TCRIMINAL*));
    return new_node;
}

TCRIMINAL        * cloneList    ( TCRIMINAL       * src )
{
    TCRIMINAL * ptr = src, *next = nullptr, *result;
    if(ptr != nullptr)
        next = ptr->m_Next;
    while(ptr != nullptr){
        TCRIMINAL* copy = copy_of_node(ptr);
        ptr->m_Next = copy;
        ptr = next;
        if(next != nullptr)
            next = next->m_Next;
    }
    // Copy contact list
    ptr = src, next = src->m_Next;
    while(ptr != nullptr){
        for(size_t i = 0; i < ptr->m_Cnt; i++){
            next->m_Contacts[i] = ptr->m_Contacts[i]->m_Next;
        }
        ptr = next->m_Next;
        if(ptr != nullptr)
            next = ptr->m_Next;
    }
    //split ll
    ptr = src, next = ptr->m_Next, result = next;
    while(next != nullptr){
        ptr->m_Next = next->m_Next;
        ptr = ptr->m_Next;
        if(ptr != nullptr)
            next->m_Next = ptr->m_Next;
        next = next->m_Next;
    }
    return result;
}
void               addContact   ( TCRIMINAL       * dst,
                                  TCRIMINAL       * contact )
{
    dst->m_Contacts[dst->m_Cnt++] = contact;
    if(dst->m_Cnt == dst->m_Capacity){
        dst->m_Capacity *= 2;
        dst->m_Contacts = (TCRIMINAL**) realloc(dst->m_Contacts, dst->m_Capacity * sizeof(TCRIMINAL*));
    }
}
void               freeList     ( TCRIMINAL       * src )
{
    TCRIMINAL * ptr = src, *next = nullptr;
    if(ptr->m_Next != nullptr)
        next = ptr->m_Next;
    while(ptr != nullptr){
        free(ptr->m_Name);
        free(ptr->m_Contacts);
        free(ptr);
        ptr = next;
        if(next != nullptr)
            next = next->m_Next;
    }
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
    TCRIMINAL * a, *b;
    char tmp[100];

    assert ( sizeof ( TCRIMINAL ) == 3 * sizeof ( void * ) + 2 * sizeof ( size_t ) );
    a = nullptr;
    a = createRecord ( "Peter", a );
    a = createRecord ( "John", a );
    a = createRecord ( "Joe", a );
    a = createRecord ( "Maria", a );
    addContact ( a, a -> m_Next );
    addContact ( a -> m_Next -> m_Next, a -> m_Next -> m_Next -> m_Next );
    addContact ( a -> m_Next -> m_Next -> m_Next, a -> m_Next );
    assert ( a
             && ! strcmp ( a -> m_Name, "Maria" )
             && a -> m_Cnt == 1
             && a -> m_Contacts[0] == a -> m_Next );
    assert ( a -> m_Next
             && ! strcmp ( a -> m_Next -> m_Name, "Joe" )
             && a -> m_Next -> m_Cnt == 0 );
    assert ( a -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "John" )
             && a -> m_Next -> m_Next -> m_Cnt == 1
             && a -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
             && a -> m_Next -> m_Next -> m_Next -> m_Cnt == 1
             && a -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    b = cloneList ( a );
    strcpy ( tmp, "Moe" );
    a = createRecord ( tmp, a );
    strcpy ( tmp, "Victoria" );
    a = createRecord ( tmp, a );
    strcpy ( tmp, "Peter" );
    a = createRecord ( tmp, a );
    addContact ( b -> m_Next -> m_Next -> m_Next, b -> m_Next -> m_Next );
    assert ( a
             && ! strcmp ( a -> m_Name, "Peter" )
             && a -> m_Cnt == 0 );
    assert ( a -> m_Next
             && ! strcmp ( a -> m_Next -> m_Name, "Victoria" )
             && a -> m_Next -> m_Cnt == 0 );
    assert ( a -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "Moe" )
             && a -> m_Next -> m_Next -> m_Cnt == 0 );
    assert ( a -> m_Next -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Maria" )
             && a -> m_Next -> m_Next -> m_Next -> m_Cnt == 1
             && a -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Joe" )
             && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cnt == 0 );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "John" )
             && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cnt == 1
             && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
             && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cnt == 1
             && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( b
             && ! strcmp ( b -> m_Name, "Maria" )
             && b -> m_Cnt == 1
             && b -> m_Contacts[0] == b -> m_Next );
    assert ( b -> m_Next
             && ! strcmp ( b -> m_Next -> m_Name, "Joe" )
             && b -> m_Next -> m_Cnt == 0 );
    assert ( b -> m_Next -> m_Next
             && ! strcmp ( b -> m_Next -> m_Next -> m_Name, "John" )
             && b -> m_Next -> m_Next -> m_Cnt == 1
             && b -> m_Next -> m_Next -> m_Contacts[0] == b -> m_Next -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next
             && ! strcmp ( b -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
             && b -> m_Next -> m_Next -> m_Next -> m_Cnt == 2
             && b -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == b -> m_Next
             && b -> m_Next -> m_Next -> m_Next -> m_Contacts[1] == b -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    freeList ( a );
    addContact ( b -> m_Next, b -> m_Next );
    a = cloneList ( b );
    assert ( a
             && ! strcmp ( a -> m_Name, "Maria" )
             && a -> m_Cnt == 1
             && a -> m_Contacts[0] == a -> m_Next );
    assert ( a -> m_Next
             && ! strcmp ( a -> m_Next -> m_Name, "Joe" )
             && a -> m_Next -> m_Cnt == 1
             && a -> m_Next -> m_Contacts[0] == a -> m_Next );
    assert ( a -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "John" )
             && a -> m_Next -> m_Next -> m_Cnt == 1
             && a -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
             && a -> m_Next -> m_Next -> m_Next -> m_Cnt == 2
             && a -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next
             && a -> m_Next -> m_Next -> m_Next -> m_Contacts[1] == a -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( b
             && ! strcmp ( b -> m_Name, "Maria" )
             && b -> m_Cnt == 1
             && b -> m_Contacts[0] == b -> m_Next );
    assert ( b -> m_Next
             && ! strcmp ( b -> m_Next -> m_Name, "Joe" )
             && b -> m_Next -> m_Cnt == 1
             && b -> m_Next -> m_Contacts[0] == b -> m_Next );
    assert ( b -> m_Next -> m_Next
             && ! strcmp ( b -> m_Next -> m_Next -> m_Name, "John" )
             && b -> m_Next -> m_Next -> m_Cnt == 1
             && b -> m_Next -> m_Next -> m_Contacts[0] == b -> m_Next -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next
             && ! strcmp ( b -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
             && b -> m_Next -> m_Next -> m_Next -> m_Cnt == 2
             && b -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == b -> m_Next
             && b -> m_Next -> m_Next -> m_Next -> m_Contacts[1] == b -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    freeList ( b );
    freeList ( a );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
