SDSTR_ERRS sdstr_initialize(SDSTR_CREATE_FILE_FUNC createFileFunc,
					  SDSTR_DELETE_FILE_FUNC deleteFileFunc,
					  SDSTR_OPEN_FILE_FUNC openFileFunc,
					  SDSTR_CLOSE_FILE_FUNC closeFileFunc,
					  SDSTR_WRITE_FUNC writeFunc,
					  SDSTR_READ_FUNC readFunc)
{
	if (createFileFunc == nullptr ||
		deleteFileFunc == nullptr ||
		openFileFunc == nullptr ||
		closeFileFunc == nullptr ||
		writeFunc == nullptr ||
		readFunc == nullptr)
	{
		return SDSTR_ERR_INIT_FUNC_NULL;
	}
	
	g_SDSTR_OpenFileFunc = openFileFunc;
	g_SDSTR_CloseFileFunc = closeFileFunc;
	g_SDSTR_CreateFileFunc = createFileFunc;
	g_SDSTR_DeleteFileFunc = deleteFileFunc;
	g_SDSTR_WriteFunc = writeFunc;
	g_SDSTR_ReadFunc = readFunc;
	
	return SDSTR_ERR_NONE;
}

void sdstr_finalize()
{
	g_SDSTR_Began = false;
	
	g_SDSTR_OpenFileFunc = nullptr;
	g_SDSTR_CloseFileFunc = nullptr;
	g_SDSTR_CreateFileFunc = nullptr;
	g_SDSTR_DeleteFileFunc = nullptr;
	g_SDSTR_WriteFunc = nullptr;
	g_SDSTR_ReadFunc = nullptr;
	
	return SDSTR_ERR_NONE;
}

SDSTR_ERRS sdstr_begin(const char* fileName, uint32_t startIndex)
{
	if (g_SDSTR_Began)
	{
		return SDSTR_ERR_ALREADY_BEGAN;
	}
	
	g_CurrentCharIndex = startIndex; // keep track of where we are going to put characters.
	
	g_SDSTR_Began = true; // flag the sdstr is open for streaming.
	g_SDSTR_CreateFileFunc(fileName); // make sure file exists.
	g_SDSTR_OpenFileFunc(fileName); // open the file.
	
	return SDSTR_ERR_NONE;
}

SDSTR_ERRS sdstr_end(const char* fileName)
{
	if (!g_SDSTR_Began)
	{
		return SDSTR_ERR_NOT_BEGAN;
	}
	
	g_CurrentCharIndex = 0;
	
	g_SDSTR_CloseFileFunc(fileName);
	g_SDSTR_Began = false;
	
	return SDSTR_ERR_NONE;
}

SDSTR_ERRS sdstr_put(char c)
{
	if (!g_SDSTR_Began)
	{
		return SDSTR_ERR_NOT_BEGAN;
	}
	
	g_SDSTR_WriteFunc(g_CurrentCharIndex, c);
	g_CurrentCharIndex++;
	
	return SDSTR_ERR_NONE;
}

char sdstr_get()
{
	if (!g_SDSTR_Began)
	{
		return '\0';
	}
	
	g_CurrentCharIndex++;
	return g_SDSTR_ReadFunc(g_CurrentCharIndex - 1);
}



char * file;

int main()
{
	file = new char[2001];
	
	sdstr_initialize([](const char* filename) {
		printf("Creating file %s\r\n", filename);
	},
					[](const char* filename) {
		printf("Deleting file %s\r\n", filename);
	},
					 [](const char* filename) {
		printf("Opening file %s\r\n", filename);
	},
					 [](const char* filename) {
		printf("Closing file %s\r\n", filename);
	},
					 [](uint32_t bpos, char c) {
		printf("[%i] %c\r\n", bpos, c);
		file[bpos] = c;
	},
					 [](uint32_t bpos) {
		return file[bpos];
	});
	
	
	
	sdstr_begin("testfile.txt", 0);
	sdstr_put('A');
	sdstr_put('A');
	sdstr_put('A');
	sdstr_put('A');
	sdstr_put('A');
	sdstr_put('A');
	sdstr_put('A');
	sdstr_end("testfile.txt");
	
	sdstr_begin("testfile.txt", 0);
	for (int i = 0; i < 7; i++)
	{
		char c = sdstr_get();
		printf("READ FROM SDSTR: %c\r\n", c);
	}
	sdstr_end("testfile.txt");
	
	sdstr_finalize();
	
	delete[] file;
	
	return 0;
}
