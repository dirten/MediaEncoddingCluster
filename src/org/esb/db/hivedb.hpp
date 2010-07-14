#ifndef hivedb_hpp
#define hivedb_hpp
#include "litesql.hpp"
namespace db {
class Project;
class MediaFile;
class Profile;
class Stream;
class MediaFileProjectRelation {
public:
    class Row {
    public:
        litesql::Field<int> project;
        litesql::Field<int> mediaFile;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType MediaFile;
    static const litesql::FieldType Project;
    static void link(const litesql::Database& db, const db::MediaFile& o0, const db::Project& o1);
    static void unlink(const litesql::Database& db, const db::MediaFile& o0, const db::Project& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<MediaFileProjectRelation::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class ProfileProjectRelation {
public:
    class Row {
    public:
        litesql::Field<int> project;
        litesql::Field<int> profile;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Profile;
    static const litesql::FieldType Project;
    static void link(const litesql::Database& db, const db::Profile& o0, const db::Project& o1);
    static void unlink(const litesql::Database& db, const db::Profile& o0, const db::Project& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<ProfileProjectRelation::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class MediaFileStreamRelation {
public:
    class Row {
    public:
        litesql::Field<int> stream;
        litesql::Field<int> mediaFile;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType MediaFile;
    static const litesql::FieldType Stream;
    static void link(const litesql::Database& db, const db::MediaFile& o0, const db::Stream& o1);
    static void unlink(const litesql::Database& db, const db::MediaFile& o0, const db::Stream& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<MediaFileStreamRelation::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class Project : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class MediafilesHandle : public litesql::RelationHandle<Project> {
    public:
        MediafilesHandle(const Project& owner);
        void link(const MediaFile& o0);
        void unlink(const MediaFile& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<MediaFile> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<MediaFileProjectRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class ProfilesHandle : public litesql::RelationHandle<Project> {
    public:
        ProfilesHandle(const Project& owner);
        void link(const Profile& o0);
        void unlink(const Profile& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Profile> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<ProfileProjectRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Name;
    litesql::Field<std::string> name;
    static const litesql::FieldType Created;
    litesql::Field<litesql::Date> created;
protected:
    void defaults();
public:
    Project(const litesql::Database& db);
    Project(const litesql::Database& db, const litesql::Record& rec);
    Project(const Project& obj);
    const Project& operator=(const Project& obj);
    Project::MediafilesHandle mediafiles();
    Project::ProfilesHandle profiles();
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect();
    std::auto_ptr<Project> upcast();
    std::auto_ptr<Project> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, Project o);
class MediaFile : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class ProjectHandle : public litesql::RelationHandle<MediaFile> {
    public:
        ProjectHandle(const MediaFile& owner);
        void link(const Project& o0);
        void unlink(const Project& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Project> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<MediaFileProjectRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class StreamsHandle : public litesql::RelationHandle<MediaFile> {
    public:
        StreamsHandle(const MediaFile& owner);
        void link(const Stream& o0);
        void unlink(const Stream& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Stream> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<MediaFileStreamRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Filename;
    litesql::Field<std::string> filename;
    static const litesql::FieldType Path;
    litesql::Field<std::string> path;
    static const litesql::FieldType Filesize;
    litesql::Field<double> filesize;
    static const litesql::FieldType Streamcount;
    litesql::Field<int> streamcount;
    static const litesql::FieldType Containertype;
    litesql::Field<std::string> containertype;
    static const litesql::FieldType Duration;
    litesql::Field<double> duration;
    static const litesql::FieldType Bitrate;
    litesql::Field<int> bitrate;
    static const litesql::FieldType Created;
    litesql::Field<litesql::DateTime> created;
    static const litesql::FieldType Filetype;
    litesql::Field<int> filetype;
    static const litesql::FieldType Parent;
    litesql::Field<int> parent;
protected:
    void defaults();
public:
    MediaFile(const litesql::Database& db);
    MediaFile(const litesql::Database& db, const litesql::Record& rec);
    MediaFile(const MediaFile& obj);
    const MediaFile& operator=(const MediaFile& obj);
    MediaFile::ProjectHandle project();
    MediaFile::StreamsHandle streams();
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect();
    std::auto_ptr<MediaFile> upcast();
    std::auto_ptr<MediaFile> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, MediaFile o);
class Profile : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class ProjectHandle : public litesql::RelationHandle<Profile> {
    public:
        ProjectHandle(const Profile& owner);
        void link(const Project& o0);
        void unlink(const Project& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Project> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<ProfileProjectRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Name;
    litesql::Field<std::string> name;
    static const litesql::FieldType Created;
    litesql::Field<litesql::DateTime> created;
    static const litesql::FieldType Format;
    litesql::Field<std::string> format;
    static const litesql::FieldType Formatext;
    litesql::Field<std::string> formatext;
    static const litesql::FieldType Vcodec;
    litesql::Field<int> vcodec;
    static const litesql::FieldType Vbitrate;
    litesql::Field<int> vbitrate;
    static const litesql::FieldType Vframerate;
    litesql::Field<int> vframerate;
    static const litesql::FieldType Vwidth;
    litesql::Field<int> vwidth;
    static const litesql::FieldType Vheight;
    litesql::Field<int> vheight;
    static const litesql::FieldType Vextra;
    litesql::Field<std::string> vextra;
    static const litesql::FieldType Acodec;
    litesql::Field<int> acodec;
    static const litesql::FieldType Achannels;
    litesql::Field<int> achannels;
    static const litesql::FieldType Abitrate;
    litesql::Field<int> abitrate;
    static const litesql::FieldType Asamplerate;
    litesql::Field<int> asamplerate;
    static const litesql::FieldType Aextra;
    litesql::Field<std::string> aextra;
    static const litesql::FieldType Profiletype;
    litesql::Field<int> profiletype;
protected:
    void defaults();
public:
    Profile(const litesql::Database& db);
    Profile(const litesql::Database& db, const litesql::Record& rec);
    Profile(const Profile& obj);
    const Profile& operator=(const Profile& obj);
    Profile::ProjectHandle project();
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect();
    std::auto_ptr<Profile> upcast();
    std::auto_ptr<Profile> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, Profile o);
class Stream : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class MediafileHandle : public litesql::RelationHandle<Stream> {
    public:
        MediafileHandle(const Stream& owner);
        void link(const MediaFile& o0);
        void unlink(const MediaFile& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<MediaFile> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<MediaFileStreamRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Streamindex;
    litesql::Field<int> streamindex;
    static const litesql::FieldType Streamtype;
    litesql::Field<int> streamtype;
    static const litesql::FieldType Codecid;
    litesql::Field<int> codecid;
    static const litesql::FieldType Codecname;
    litesql::Field<std::string> codecname;
    static const litesql::FieldType Frameratenum;
    litesql::Field<int> frameratenum;
    static const litesql::FieldType Framerateden;
    litesql::Field<int> framerateden;
    static const litesql::FieldType Streamtimebasenum;
    litesql::Field<int> streamtimebasenum;
    static const litesql::FieldType Streamtimebaseden;
    litesql::Field<int> streamtimebaseden;
    static const litesql::FieldType Codectimebasenum;
    litesql::Field<int> codectimebasenum;
    static const litesql::FieldType Codectimebaseden;
    litesql::Field<int> codectimebaseden;
    static const litesql::FieldType Firstpts;
    litesql::Field<double> firstpts;
    static const litesql::FieldType Firstdts;
    litesql::Field<double> firstdts;
    static const litesql::FieldType Duration;
    litesql::Field<double> duration;
    static const litesql::FieldType Nbframes;
    litesql::Field<double> nbframes;
    static const litesql::FieldType Ticksperframe;
    litesql::Field<int> ticksperframe;
    static const litesql::FieldType Framecount;
    litesql::Field<int> framecount;
    static const litesql::FieldType Width;
    litesql::Field<int> width;
    static const litesql::FieldType Height;
    litesql::Field<int> height;
    static const litesql::FieldType Gopsize;
    litesql::Field<int> gopsize;
    static const litesql::FieldType Pixfmt;
    litesql::Field<int> pixfmt;
    static const litesql::FieldType Bitrate;
    litesql::Field<int> bitrate;
    static const litesql::FieldType Samplerate;
    litesql::Field<int> samplerate;
    static const litesql::FieldType Samplefmt;
    litesql::Field<int> samplefmt;
    static const litesql::FieldType Channels;
    litesql::Field<int> channels;
    static const litesql::FieldType Bitspercodedsample;
    litesql::Field<int> bitspercodedsample;
    static const litesql::FieldType Privdatasize;
    litesql::Field<int> privdatasize;
    static const litesql::FieldType Privdata;
    litesql::Field<std::string> privdata;
    static const litesql::FieldType Extradatasize;
    litesql::Field<int> extradatasize;
    static const litesql::FieldType Extradata;
    litesql::Field<std::string> extradata;
    static const litesql::FieldType Flags;
    litesql::Field<int> flags;
    static const litesql::FieldType Extraprofileflags;
    litesql::Field<std::string> extraprofileflags;
protected:
    void defaults();
public:
    Stream(const litesql::Database& db);
    Stream(const litesql::Database& db, const litesql::Record& rec);
    Stream(const Stream& obj);
    const Stream& operator=(const Stream& obj);
    Stream::MediafileHandle mediafile();
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect();
    std::auto_ptr<Stream> upcast();
    std::auto_ptr<Stream> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, Stream o);
class HiveDb : public litesql::Database {
public:
    HiveDb(std::string backendType, std::string connInfo);
protected:
    virtual std::vector<litesql::Database::SchemaItem> getSchema() const;
    static void initialize();
};
}
#endif
