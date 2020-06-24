#include <stdint.h>
#include <stdio.h>

// sdsql_sd_callback is assigned by the user, its a function that writes or reads strings from
// any source.
typedef void(*SDSTR_WRITE_FUNC)(uint32_t bytePosition, char c);
typedef char(*SDSTR_READ_FUNC)(uint32_t bytePosition);

typedef void(*SDSTR_CREATE_FILE_FUNC)(const char * filename);
typedef void(*SDSTR_DELETE_FILE_FUNC)(const char * filename);

typedef void(*SDSTR_OPEN_FILE_FUNC)(const char * filename);
typedef void(*SDSTR_CLOSE_FILE_FUNC)(const char * filename);

enum SDSTR_ERRS
{
	SDSTR_ERR_NONE,
	SDSTR_ERR_INIT_FUNC_NULL,
	SDSTR_ERR_ALREADY_BEGAN,
	SDSTR_ERR_NOT_BEGAN,
};


extern SDSTR_ERRS sdstr_initialize(SDSTR_CREATE_FILE_FUNC createFileFunc,
							 SDSTR_DELETE_FILE_FUNC deleteFileFunc,
							 SDSTR_OPEN_FILE_FUNC openFileFunc,
							 SDSTR_CLOSE_FILE_FUNC closeFileFunc,
							 SDSTR_WRITE_FUNC writeFunc,
							 SDSTR_READ_FUNC readFunc);
extern void sdstr_finalize();

extern SDSTR_ERRS sdstr_begin(const char* fileName, uint32_t startIndex);
extern SDSTR_ERRS sdstr_end(const char* fileName);
extern SDSTR_ERRS sdstr_put(char c);
extern char sdstr_get();


// globals
static SDSTR_CREATE_FILE_FUNC g_SDSTR_OpenFileFunc = nullptr;
static SDSTR_DELETE_FILE_FUNC g_SDSTR_CloseFileFunc = nullptr;
static SDSTR_CREATE_FILE_FUNC g_SDSTR_CreateFileFunc = nullptr;
static SDSTR_DELETE_FILE_FUNC g_SDSTR_DeleteFileFunc = nullptr;
static SDSTR_WRITE_FUNC g_SDSTR_WriteFunc = nullptr;
static SDSTR_READ_FUNC g_SDSTR_ReadFunc = nullptr;

static uint32_t g_CurrentCharIndex;

static bool g_SDSTR_Began = false; // this is used to ensure SD card file is open.
