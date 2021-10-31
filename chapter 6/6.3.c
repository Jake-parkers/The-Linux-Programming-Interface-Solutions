/**
 * The setenv() function creates a new environment variable by allocating a memory
buffer for a string of the form name=value, and copying the strings pointed to by
name and value into that buffer. Note that we don’t need to (in fact, must not)
supply an equal sign at the end of name or the start of value, because setenv() adds
this character when it adds the new definition to the environment.
* The setenv() function doesn’t change the environment if the variable identified
by name already exists and overwrite has the value 0. If overwrite is nonzero, the envi-
ronment is always changed.
* The fact that setenv() copies its arguments means that, unlike with putenv(), we
can subsequently modify the contents of the strings pointed to by name and value
without affecting the environment. It also means that using automatic variables as
arguments to setenv() doesn’t cause any problems.
 */ 

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
extern char **environ;

int mysetenv(const char *name, const char *value, int overwrite) {
    // if the env var already exists and overwrite isn't specified, do nothing
    if ((getenv(name) != NULL) && overwrite == 0) {
        printf("Value already exists\n");
        return 0;
    }
    // else 
    size_t len = strlen(name);
    len = len + strlen(value);
    printf("Len = %d\n", len);
    char *buf = malloc((len + 1) * sizeof(char));
    strcat(buf, name);
    strcat(buf, "=");
    strcat(buf, value);
    printf("Buf = %s\n", buf);
    if (putenv(buf) != 0) {
        printf("OOps an error occurred setting %s",buf);
        free(buf);
        return -1;
    }
    //free(buf);
    return 0;
}

int myunsetenv(const char *name) {
    const char *env_val = getenv(name);

    if (getenv(name) == NULL) {
        printf("Value does not exist\n");
        return -1;
    }

    size_t len = strlen(name);
    len = len + strlen(env_val);

    char *buf = malloc((len + 1) * sizeof(char));

    strcat(buf, name);
    strcat(buf, "=");
    strcat(buf, env_val);
    printf("New1 = %s\n", buf);
    char **environ_var;
    for (environ_var = environ; *environ_var != NULL; environ_var++)
    {
        if (strcmp(*environ_var, buf) == 0) putenv(name);
        
    }
    return 1;
}

int main() {
    char *key = "Name";
    char *value = "Joshua";

    mysetenv(key, value, 0);
   
   
    myunsetenv(key);
    if (getenv(key) == NULL) {
        printf("Value does not exist\n");
    }
}