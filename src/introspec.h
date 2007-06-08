
/**
*if(instanceOf(*fis,FormatInputStream)){
*   cout << "Format InputStream here"<<endl;
*}
*/
#define instanceOf(o1, o2)(strcmp(typeid(o1).name(),typeid(o2).name())==0)

