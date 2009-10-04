#include <string>

#include "StackDumper.h"
#include "org/esb/util/Log.h"

bool MyDumpSender(const wchar_t* dump_path,
                             const wchar_t* minidump_id,
                             void* context,
                             EXCEPTION_POINTERS* exinfo,
                             MDRawAssertionInfo* assertion,
							 bool succeeded){
	logdebug("Sending CrashReport");
	std::string chkpfile="checkpoint.txt";

	std::string url="http://188.40.40.157/submit";
	std::wstring dumpfile=dump_path;
	std::string t="/";
	std::string d=".";
	std::string dmp="dmp";
	std::string p="ProductName";
	std::string v="Version";
	std::string product="MediaEncodingCluster";
	std::string version="0.0.4.5";

	std::wstring wpro(product.begin(), product.end());
	std::wstring wver(version.begin(), version.end());
	std::wstring wp(p.begin(),p.end());
	std::wstring wv(v.begin(),v.end());

	dumpfile.append(std::wstring(t.begin(), t.end()));
	dumpfile.append(minidump_id);
	dumpfile.append(std::wstring(d.begin(), d.end()));
	dumpfile.append(std::wstring(dmp.begin(), dmp.end()));
	logdebug(std::string(dumpfile.begin(), dumpfile.end()));
	std::map<std::wstring, std::wstring> para;
	para[wp]=wpro;
	para[wv]=wver;
	std::wstring wresult;
	google_breakpad::CrashReportSender sender(std::wstring(chkpfile.begin(), chkpfile.end()));
	google_breakpad::ReportResult r=sender.SendCrashReport(std::wstring(url.begin(), url.end()),para,dumpfile,&wresult);
	std::string result(wresult.begin(), wresult.end());
	logdebug("CrashReport sended : "<<result<<":::"<<r);
	return true;
}

StackDumper::StackDumper(std::string dmp_path):
	exhandler(
#ifndef WIN32
		dmp_path,
#elif WIN32
		std::wstring(dmp_path.begin(), dmp_path.end()),
#endif
		NULL,
		&MyDumpSender,
		NULL,
#ifndef WIN32
		true
#elif WIN32
		google_breakpad::ExceptionHandler::HANDLER_ALL
#endif
		){

}
bool StackDumper::DumpSender(const wchar_t* dump_path,
                             const wchar_t* minidump_id,
                             void* context,
                             EXCEPTION_POINTERS* exinfo,
                             MDRawAssertionInfo* assertion,
							 bool succeeded){
	logdebug("Sending CrashReport");

	return true;
}
StackDumper::~StackDumper(){

}

