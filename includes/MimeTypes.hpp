
#ifndef WEBSERV_MIMETYPES_HPP
#define WEBSERV_MIMETYPES_HPP

#include <string>

class MimeTypes {
public:
	static const char* getType(const char * path);
	static std::string getType(const std::string &extension);
	static const char *getExtension(const char *type, int skip = 0);
	static std::string getExtension(const std::string &type, int skip = 0);

private:
	struct entry {
		const char* fileExtension;
		const char* mimeType;
	};
	static MimeTypes::entry types[347];
	static int strcmpi(const char *s1, const char *s2);

};

#endif //WEBSERV_MIMETYPES_HPP
