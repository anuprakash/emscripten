#include <stdio.h>
#include <string.h>
#include <emscripten/fetch.h>

void downloadSucceeded(emscripten_fetch_t *fetch)
{
  printf("Finished downloading %llu bytes from URL %s.\n", fetch->numBytes, fetch->url);
  // The data is now available at fetch->data[0] through fetch->data[fetch->numBytes-1];
  emscripten_fetch_close(fetch); // Free data associated with the fetch.
}

void downloadFailed(emscripten_fetch_t *fetch)
{
  printf("Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
  emscripten_fetch_close(fetch); // Also free data on failure.
}

void downloadProgress(emscripten_fetch_t *fetch)
{
  printf("Downloading %s.. %.2f%% complete. HTTP readyState: %d. HTTP status: %d.\n"
    "HTTP statusText: %s. Received chunk [%llu, %llu[\n", 
    fetch->url, (fetch->dataOffset + fetch->numBytes) * 100.0 / fetch->totalBytes,
    fetch->readyState, fetch->status, fetch->statusText,
    fetch->dataOffset, fetch->dataOffset + fetch->numBytes);

  // Process the partial data stream fetch->data[0] thru fetch->data[fetch->numBytes-1]
  // This buffer represents the file at offset fetch->dataOffset.
  for(size_t i = 0; i < fetch->numBytes; ++i)
    ; // Process fetch->data[i];
}

int main()
{
  emscripten_fetch_attr_t attr;
  emscripten_fetch_attr_init(&attr);
  strcpy(attr.requestMethod, "GET");
  attr.attributes = EMSCRIPTEN_FETCH_STREAM_DATA;
  attr.onsuccess = downloadSucceeded;
  attr.onprogress = downloadProgress;
  attr.onerror = downloadFailed;
  attr.timeoutMsecs = 2*60;
  emscripten_fetch(&attr, "myfile.dat");
}
