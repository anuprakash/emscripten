#include <string.h>
#include <stdio.h>
#include <emscripten/fetch.h>

void success(emscripten_fetch_t *fetch)
{
  printf("Deleting file from IDB succeeded.\n");
  emscripten_fetch_close(fetch);
}

void failure(emscripten_fetch_t *fetch)
{
  printf("Deleting file from IDB failed.\n");
  emscripten_fetch_close(fetch);
}

int main()
{
  emscripten_fetch_attr_init(&attr);
  strcpy(attr.requestMethod, "EM_IDB_DELETE");
  emscripten_fetch(&attr, "filename_to_delete.dat");
}
