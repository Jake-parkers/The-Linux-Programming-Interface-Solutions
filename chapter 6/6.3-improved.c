#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

extern char **environ;

// If this variable is not a null pointer we allocated the current environment
static char **last_environ;

int setenv(const char *name, const char *value, int overwrite) {
    register char **ep = 0;
    const size_t nameLength = strlen(name);
    const size_t valueLength = strlen(value) + 1;

    char *name_dup = strdup(name);

    char *existingEnvVar = getenv(name_dup);

   // if the env var already exists and overwrite isn't specified, do nothing
    if (existingEnvVar != NULL && overwrite == 0) {
        printf("Value already exists\n");
        return 0;
    }

    /**
     * Env var doesn't exist
     * Allocate new memory buffer to hold string of the form name=value.
     * Copy strings pointed to by name and value to the newly allocated buffer
     * set the env var using put env
     */
    if (existingEnvVar == NULL) {
        char *newEnvVar = malloc(nameLength + valueLength + 1);
        memcpy(newEnvVar, name, nameLength);
        newEnvVar[nameLength] = '=';
        memcpy(&newEnvVar[nameLength + 1], value, valueLength);

        return putenv(newEnvVar);
    }

    /**
     * Env var exists
     * check if the memory allocated to the current env var pair is enough to hold the env var and its new value
     * if it isn't, allocate a new memory and modify the pointer
     * if it is, copy over the value to replace the old value
     */ 
    if (existingEnvVar != NULL && overwrite > 0) {
        size_t existingEnvTotalLen = strlen(existingEnvVar) + nameLength + 1;
        size_t newEnvTotalLen = nameLength + valueLength + 1;

        // get env var from **environment
        for (ep = environ; *ep != NULL; ep++) {
           if (strncmp(*ep, name, nameLength) == 0 && (*ep)[nameLength] == '=') {
               break;
           }
        }
        printf("Env = %s \n", *ep);
        // not enough memory, so allocate and  re-point the env var
        if (existingEnvTotalLen < newEnvTotalLen) {
            char *buf = malloc((newEnvTotalLen) * sizeof(char));
            if (buf == NULL) {
                errno = ENOMEM;
                return -1;
            }
            // point old env var pointer to newly allocated memory
            *ep = buf;
        } 

        memcpy(*ep, name, nameLength);
        (*ep)[nameLength] = '=';
        memcpy(&(*ep)[nameLength + 1], value, valueLength);
    }
}

int myunsetenv(const char *name) {
    char *name_dup = strdup(name);

    char *env_val = getenv(name_dup);

    if (name_dup == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (env_val == NULL) {
        printf("Value does not exist\n");
        return 0;
    }

    // name exists in env var, so we remove it by calling putenv with a string that has no '='
    /**
     * The GNU C library implementation provides a nonstandard extension.  If string does not include an equal sign:

           putenv("NAME");

       then the named variable is removed from the caller's environment.

     */ 
    if(putenv(name_dup) == 0) return 0;
    
    // alternatively, we look for the pointer to the env variable and set it to the NULL
    return -1;
}

int main() {
    char *name = "NODE_ENV";
    char *value = "development";

    setenv(name, value, 0);

    printf("Name = %s, value = %s\n", name, getenv(name));

    value = "production";
    setenv(name, value, 0);
    printf("Name = %s, value = %s\n", name, getenv(name));

    setenv(name, value, 1);
    printf("Name = %s, value = %s\n", name, getenv(name));

    unsetenv(name);
    printf("Name = %s, value = %s\n", name, getenv(name));
}