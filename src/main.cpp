
#include "../header/curl.h"
#include "../header/easy.h"

struct MemoryStruct {
	char *memory;
	size_t size;
	char *memoryHeader;
	size_t sizeHeader;
	CURLcode ret;
};
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	char *ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
	if (!ptr) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}
static size_t header_callback(char *contents, size_t size,
	size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	char *ptr = (char*)realloc(mem->memoryHeader, mem->sizeHeader + realsize + 1);
	if (!ptr) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memoryHeader = ptr;
	memcpy(&(mem->memoryHeader[mem->sizeHeader]), contents, realsize);
	mem->sizeHeader += realsize;
	mem->memoryHeader[mem->sizeHeader] = 0;

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;
	
	return realsize;
}

extern "C" int testInt(int i, int j) {
	return i + j;
}
extern "C" void memFree(void* p) {
	free(p);
}
	
extern "C" char* curl_get(char* url, char* method, char* sendData, int sendLen, int httpVer, int *num, int *headerNum, char* addHeader0, char* addHeader1, char* addHeader2, char* addHeader3, char* addHeader4, char* addHeader5, char* addHeader6, char* addHeader7, char* addHeader8, char* addHeader9, char* addHeader10, char* addHeader11) {
	char* ret = (char*)"";
	CURL *data = curl_easy_init();
	
	curl_easy_setopt(data, CURLOPT_URL, url);
	curl_easy_setopt(data, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(data, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(data, CURLOPT_HTTP_VERSION, httpVer);
	curl_easy_setopt(data, CURLOPT_TIMEOUT, 20);
	
	struct curl_slist *list = NULL;
	if (addHeader0 != NULL && strcmp(addHeader0, "") != 0)
		list = curl_slist_append(list, addHeader0);
	if (addHeader1 != NULL && strcmp(addHeader1, "") != 0)
		list = curl_slist_append(list, addHeader1);
	if (addHeader2 != NULL && strcmp(addHeader2, "") != 0)
		list = curl_slist_append(list, addHeader2);
	if (addHeader3 != NULL && strcmp(addHeader3, "") != 0)
		list = curl_slist_append(list, addHeader3);
	if (addHeader4 != NULL && strcmp(addHeader4, "") != 0)
		list = curl_slist_append(list, addHeader4);
	if (addHeader5 != NULL && strcmp(addHeader5, "") != 0)
		list = curl_slist_append(list, addHeader5);
	if (addHeader6 != NULL && strcmp(addHeader6, "") != 0)
		list = curl_slist_append(list, addHeader6);
	if (addHeader7 != NULL && strcmp(addHeader7, "") != 0)
		list = curl_slist_append(list, addHeader7);
	if (addHeader8 != NULL && strcmp(addHeader8, "") != 0)
		list = curl_slist_append(list, addHeader8);
	if (addHeader9 != NULL && strcmp(addHeader9, "") != 0)
		list = curl_slist_append(list, addHeader9);
	if (addHeader10 != NULL && strcmp(addHeader10, "") != 0)
		list = curl_slist_append(list, addHeader10);
	if (addHeader11 != NULL && strcmp(addHeader11, "") != 0)
		list = curl_slist_append(list, addHeader11);
	
	curl_easy_setopt(data, CURLOPT_HTTPHEADER, list);

	struct MemoryStruct chunk;
	
	chunk.memory = (char*)malloc(1);
	chunk.size = 0;
	chunk.memoryHeader = (char*)malloc(1);
	chunk.sizeHeader = 0;
	
	curl_easy_setopt(data, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(data, CURLOPT_WRITEDATA, (void *)&chunk);
	
	curl_easy_setopt(data, CURLOPT_HEADERFUNCTION, header_callback);
	curl_easy_setopt(data, CURLOPT_HEADERDATA, (void *)&chunk);
	
	if (strcmp(method, "GET") == 0) {
		curl_easy_setopt(data, CURLOPT_HTTPGET, 1L);
	}
	if (strcmp(method, "POST") == 0) {
		curl_easy_setopt(data, CURLOPT_POST, 1);
		curl_easy_setopt(data, CURLOPT_POSTFIELDS, sendData);
		curl_easy_setopt(data, CURLOPT_POSTFIELDSIZE, sendLen);
	}
	curl_easy_setopt(data, CURLOPT_CUSTOMREQUEST, method);
	
	CURLcode res = curl_easy_perform(data);
	
	if (res != CURLE_OK) {
		return ret;
	}

	curl_easy_cleanup(data);

	*num = chunk.size;
	*headerNum = chunk.sizeHeader;
	curl_slist_free_all(list);
	
	return chunk.memory;
}