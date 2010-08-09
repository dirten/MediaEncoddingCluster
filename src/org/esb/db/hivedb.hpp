#ifndef hivedb_hpp
#define hivedb_hpp
#include "litesql.hpp"
namespace db {
class Project;
class Filter;
class FilterParameter;
class MediaFile;
class Profile;
class Stream;
class CodecPreset;
class Config;
class Job;
class JobDetail;
class Watchfolder;
class ProcessUnit;
class FilterFilterParameterRelation {
public:
    class Row {
    public:
        litesql::Field<int> filterParameter;
        litesql::Field<int> filter;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Filter;
    static const litesql::FieldType FilterParameter;
    static void link(const litesql::Database& db, const db::Filter& o0, const db::FilterParameter& o1);
    static void unlink(const litesql::Database& db, const db::Filter& o0, const db::FilterParameter& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<FilterFilterParameterRelation::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class FilterMediaFileRelation {
public:
    class Row {
    public:
        litesql::Field<int> mediaFile;
        litesql::Field<int> filter;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Filter;
    static const litesql::FieldType MediaFile;
    static void link(const litesql::Database& db, const db::Filter& o0, const db::MediaFile& o1);
    static void unlink(const litesql::Database& db, const db::Filter& o0, const db::MediaFile& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<FilterMediaFileRelation::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class FilterProjectRelation {
public:
    class Row {
    public:
        litesql::Field<int> project;
        litesql::Field<int> filter;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Filter;
    static const litesql::FieldType Project;
    static void link(const litesql::Database& db, const db::Filter& o0, const db::Project& o1);
    static void unlink(const litesql::Database& db, const db::Filter& o0, const db::Project& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<FilterProjectRelation::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
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
class JobMediaFileRelationJobInFile {
public:
    class Row {
    public:
        litesql::Field<int> mediaFile;
        litesql::Field<int> job;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Job;
    static const litesql::FieldType MediaFile;
    static void link(const litesql::Database& db, const db::Job& o0, const db::MediaFile& o1);
    static void unlink(const litesql::Database& db, const db::Job& o0, const db::MediaFile& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<JobMediaFileRelationJobInFile::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class JobMediaFileRelationJobOutFile {
public:
    class Row {
    public:
        litesql::Field<int> mediaFile;
        litesql::Field<int> job;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Job;
    static const litesql::FieldType MediaFile;
    static void link(const litesql::Database& db, const db::Job& o0, const db::MediaFile& o1);
    static void unlink(const litesql::Database& db, const db::Job& o0, const db::MediaFile& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<JobMediaFileRelationJobOutFile::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class JobJobDetailRelationJobJobDetail {
public:
    class Row {
    public:
        litesql::Field<int> jobDetail;
        litesql::Field<int> job;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Job;
    static const litesql::FieldType JobDetail;
    static void link(const litesql::Database& db, const db::Job& o0, const db::JobDetail& o1);
    static void unlink(const litesql::Database& db, const db::Job& o0, const db::JobDetail& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<JobJobDetailRelationJobJobDetail::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class JobDetailStreamRelationJobOutStream {
public:
    class Row {
    public:
        litesql::Field<int> stream;
        litesql::Field<int> jobDetail;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType JobDetail;
    static const litesql::FieldType Stream;
    static void link(const litesql::Database& db, const db::JobDetail& o0, const db::Stream& o1);
    static void unlink(const litesql::Database& db, const db::JobDetail& o0, const db::Stream& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<JobDetailStreamRelationJobOutStream::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class JobDetailStreamRelationJobInStream {
public:
    class Row {
    public:
        litesql::Field<int> stream;
        litesql::Field<int> jobDetail;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType JobDetail;
    static const litesql::FieldType Stream;
    static void link(const litesql::Database& db, const db::JobDetail& o0, const db::Stream& o1);
    static void unlink(const litesql::Database& db, const db::JobDetail& o0, const db::Stream& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<JobDetailStreamRelationJobInStream::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class ProfileWatchfolderRelationWatchfolderProfile {
public:
    class Row {
    public:
        litesql::Field<int> watchfolder;
        litesql::Field<int> profile;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Profile;
    static const litesql::FieldType Watchfolder;
    static void link(const litesql::Database& db, const db::Profile& o0, const db::Watchfolder& o1);
    static void unlink(const litesql::Database& db, const db::Profile& o0, const db::Watchfolder& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<ProfileWatchfolderRelationWatchfolderProfile::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
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
    class FilterHandle : public litesql::RelationHandle<Project> {
    public:
        FilterHandle(const Project& owner);
        void link(const Filter& o0);
        void unlink(const Filter& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Filter> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FilterProjectRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    static const litesql::FieldType Outdirectory;
    litesql::Field<std::string> outdirectory;
    static const litesql::FieldType Status;
    litesql::Field<std::string> status;
    static const litesql::FieldType Created;
    litesql::Field<litesql::Date> created;
    static const litesql::FieldType Started;
    litesql::Field<litesql::Date> started;
    static const litesql::FieldType Completed;
    litesql::Field<litesql::Date> completed;
protected:
    void defaults();
public:
    Project(const litesql::Database& db);
    Project(const litesql::Database& db, const litesql::Record& rec);
    Project(const Project& obj);
    const Project& operator=(const Project& obj);
    Project::FilterHandle filter();
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
class Filter : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class ParameterHandle : public litesql::RelationHandle<Filter> {
    public:
        ParameterHandle(const Filter& owner);
        void link(const FilterParameter& o0);
        void unlink(const FilterParameter& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<FilterParameter> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FilterFilterParameterRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class MediafileHandle : public litesql::RelationHandle<Filter> {
    public:
        MediafileHandle(const Filter& owner);
        void link(const MediaFile& o0);
        void unlink(const MediaFile& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<MediaFile> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FilterMediaFileRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class ProjectHandle : public litesql::RelationHandle<Filter> {
    public:
        ProjectHandle(const Filter& owner);
        void link(const Project& o0);
        void unlink(const Project& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Project> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FilterProjectRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Filtername;
    litesql::Field<std::string> filtername;
    static const litesql::FieldType Filterid;
    litesql::Field<std::string> filterid;
protected:
    void defaults();
public:
    Filter(const litesql::Database& db);
    Filter(const litesql::Database& db, const litesql::Record& rec);
    Filter(const Filter& obj);
    const Filter& operator=(const Filter& obj);
    Filter::ParameterHandle parameter();
    Filter::MediafileHandle mediafile();
    Filter::ProjectHandle project();
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
    std::auto_ptr<Filter> upcast();
    std::auto_ptr<Filter> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, Filter o);
class FilterParameter : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class FilterHandle : public litesql::RelationHandle<FilterParameter> {
    public:
        FilterHandle(const FilterParameter& owner);
        void link(const Filter& o0);
        void unlink(const Filter& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Filter> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FilterFilterParameterRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Fkey;
    litesql::Field<std::string> fkey;
    static const litesql::FieldType Fval;
    litesql::Field<std::string> fval;
protected:
    void defaults();
public:
    FilterParameter(const litesql::Database& db);
    FilterParameter(const litesql::Database& db, const litesql::Record& rec);
    FilterParameter(const FilterParameter& obj);
    const FilterParameter& operator=(const FilterParameter& obj);
    FilterParameter::FilterHandle filter();
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
    std::auto_ptr<FilterParameter> upcast();
    std::auto_ptr<FilterParameter> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, FilterParameter o);
class MediaFile : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class FilterHandle : public litesql::RelationHandle<MediaFile> {
    public:
        FilterHandle(const MediaFile& owner);
        void link(const Filter& o0);
        void unlink(const Filter& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Filter> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FilterMediaFileRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    class JobsinHandle : public litesql::RelationHandle<MediaFile> {
    public:
        JobsinHandle(const MediaFile& owner);
        void link(const Job& o0);
        void unlink(const Job& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Job> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<JobMediaFileRelationJobInFile::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class JobsoutHandle : public litesql::RelationHandle<MediaFile> {
    public:
        JobsoutHandle(const MediaFile& owner);
        void link(const Job& o0);
        void unlink(const Job& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Job> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<JobMediaFileRelationJobOutFile::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    static const litesql::FieldType Metatitle;
    litesql::Field<std::string> metatitle;
    static const litesql::FieldType Metaauthor;
    litesql::Field<std::string> metaauthor;
    static const litesql::FieldType Metacopyright;
    litesql::Field<std::string> metacopyright;
    static const litesql::FieldType Metacomment;
    litesql::Field<std::string> metacomment;
    static const litesql::FieldType Metaalbum;
    litesql::Field<std::string> metaalbum;
    static const litesql::FieldType Metayear;
    litesql::Field<int> metayear;
    static const litesql::FieldType Metatrack;
    litesql::Field<int> metatrack;
    static const litesql::FieldType Metagenre;
    litesql::Field<int> metagenre;
protected:
    void defaults();
public:
    MediaFile(const litesql::Database& db);
    MediaFile(const litesql::Database& db, const litesql::Record& rec);
    MediaFile(const MediaFile& obj);
    const MediaFile& operator=(const MediaFile& obj);
    MediaFile::FilterHandle filter();
    MediaFile::ProjectHandle project();
    MediaFile::StreamsHandle streams();
    MediaFile::JobsinHandle jobsin();
    MediaFile::JobsoutHandle jobsout();
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
    class WatchfolderHandle : public litesql::RelationHandle<Profile> {
    public:
        WatchfolderHandle(const Profile& owner);
        void link(const Watchfolder& o0);
        void unlink(const Watchfolder& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Watchfolder> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<ProfileWatchfolderRelationWatchfolderProfile::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    litesql::Field<std::string> vframerate;
    static const litesql::FieldType Vwidth;
    litesql::Field<int> vwidth;
    static const litesql::FieldType Vheight;
    litesql::Field<int> vheight;
    static const litesql::FieldType Vextra;
    litesql::Field<std::string> vextra;
    static const litesql::FieldType Achannels;
    litesql::Field<int> achannels;
    static const litesql::FieldType Acodec;
    litesql::Field<int> acodec;
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
    Profile::WatchfolderHandle watchfolder();
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
    class JobsoutHandle : public litesql::RelationHandle<Stream> {
    public:
        JobsoutHandle(const Stream& owner);
        void link(const JobDetail& o0);
        void unlink(const JobDetail& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<JobDetail> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<JobDetailStreamRelationJobOutStream::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class JobsinHandle : public litesql::RelationHandle<Stream> {
    public:
        JobsinHandle(const Stream& owner);
        void link(const JobDetail& o0);
        void unlink(const JobDetail& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<JobDetail> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<JobDetailStreamRelationJobInStream::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    Stream::JobsoutHandle jobsout();
    Stream::JobsinHandle jobsin();
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
class CodecPreset : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
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
    static const litesql::FieldType Codecid;
    litesql::Field<int> codecid;
    static const litesql::FieldType Preset;
    litesql::Field<std::string> preset;
protected:
    void defaults();
public:
    CodecPreset(const litesql::Database& db);
    CodecPreset(const litesql::Database& db, const litesql::Record& rec);
    CodecPreset(const CodecPreset& obj);
    const CodecPreset& operator=(const CodecPreset& obj);
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
    std::auto_ptr<CodecPreset> upcast();
    std::auto_ptr<CodecPreset> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, CodecPreset o);
class Config : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Configkey;
    litesql::Field<std::string> configkey;
    static const litesql::FieldType Configval;
    litesql::Field<std::string> configval;
protected:
    void defaults();
public:
    Config(const litesql::Database& db);
    Config(const litesql::Database& db, const litesql::Record& rec);
    Config(const Config& obj);
    const Config& operator=(const Config& obj);
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
    std::auto_ptr<Config> upcast();
    std::auto_ptr<Config> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, Config o);
class Job : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class InputfileHandle : public litesql::RelationHandle<Job> {
    public:
        InputfileHandle(const Job& owner);
        void link(const MediaFile& o0);
        void unlink(const MediaFile& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<MediaFile> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<JobMediaFileRelationJobInFile::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class OutputfileHandle : public litesql::RelationHandle<Job> {
    public:
        OutputfileHandle(const Job& owner);
        void link(const MediaFile& o0);
        void unlink(const MediaFile& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<MediaFile> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<JobMediaFileRelationJobOutFile::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class JobdetailsHandle : public litesql::RelationHandle<Job> {
    public:
        JobdetailsHandle(const Job& owner);
        void link(const JobDetail& o0);
        void unlink(const JobDetail& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<JobDetail> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<JobJobDetailRelationJobJobDetail::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Begintime;
    litesql::Field<litesql::DateTime> begintime;
    static const litesql::FieldType Endtime;
    litesql::Field<litesql::DateTime> endtime;
    static const litesql::FieldType Status;
    litesql::Field<std::string> status;
    static const litesql::FieldType Progress;
    litesql::Field<int> progress;
protected:
    void defaults();
public:
    Job(const litesql::Database& db);
    Job(const litesql::Database& db, const litesql::Record& rec);
    Job(const Job& obj);
    const Job& operator=(const Job& obj);
    Job::InputfileHandle inputfile();
    Job::OutputfileHandle outputfile();
    Job::JobdetailsHandle jobdetails();
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
    std::auto_ptr<Job> upcast();
    std::auto_ptr<Job> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, Job o);
class JobDetail : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class JobHandle : public litesql::RelationHandle<JobDetail> {
    public:
        JobHandle(const JobDetail& owner);
        void link(const Job& o0);
        void unlink(const Job& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Job> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<JobJobDetailRelationJobJobDetail::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class OutputstreamHandle : public litesql::RelationHandle<JobDetail> {
    public:
        OutputstreamHandle(const JobDetail& owner);
        void link(const Stream& o0);
        void unlink(const Stream& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Stream> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<JobDetailStreamRelationJobOutStream::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class InputstreamHandle : public litesql::RelationHandle<JobDetail> {
    public:
        InputstreamHandle(const JobDetail& owner);
        void link(const Stream& o0);
        void unlink(const Stream& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Stream> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<JobDetailStreamRelationJobInStream::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Lastpts;
    litesql::Field<double> lastpts;
    static const litesql::FieldType Lastdts;
    litesql::Field<double> lastdts;
protected:
    void defaults();
public:
    JobDetail(const litesql::Database& db);
    JobDetail(const litesql::Database& db, const litesql::Record& rec);
    JobDetail(const JobDetail& obj);
    const JobDetail& operator=(const JobDetail& obj);
    JobDetail::JobHandle job();
    JobDetail::OutputstreamHandle outputstream();
    JobDetail::InputstreamHandle inputstream();
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
    std::auto_ptr<JobDetail> upcast();
    std::auto_ptr<JobDetail> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, JobDetail o);
class Watchfolder : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class ProfileHandle : public litesql::RelationHandle<Watchfolder> {
    public:
        ProfileHandle(const Watchfolder& owner);
        void link(const Profile& o0);
        void unlink(const Profile& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Profile> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<ProfileWatchfolderRelationWatchfolderProfile::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Infolder;
    litesql::Field<std::string> infolder;
    static const litesql::FieldType Outfolder;
    litesql::Field<std::string> outfolder;
    static const litesql::FieldType Extensionfilter;
    litesql::Field<std::string> extensionfilter;
protected:
    void defaults();
public:
    Watchfolder(const litesql::Database& db);
    Watchfolder(const litesql::Database& db, const litesql::Record& rec);
    Watchfolder(const Watchfolder& obj);
    const Watchfolder& operator=(const Watchfolder& obj);
    Watchfolder::ProfileHandle profile();
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
    std::auto_ptr<Watchfolder> upcast();
    std::auto_ptr<Watchfolder> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, Watchfolder o);
class ProcessUnit : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Sorcestream;
    litesql::Field<int> sorcestream;
    static const litesql::FieldType Targetstream;
    litesql::Field<int> targetstream;
    static const litesql::FieldType Startts;
    litesql::Field<double> startts;
    static const litesql::FieldType Endts;
    litesql::Field<double> endts;
    static const litesql::FieldType Framecount;
    litesql::Field<int> framecount;
    static const litesql::FieldType Send;
    litesql::Field<litesql::DateTime> send;
    static const litesql::FieldType Recv;
    litesql::Field<litesql::DateTime> recv;
protected:
    void defaults();
public:
    ProcessUnit(const litesql::Database& db);
    ProcessUnit(const litesql::Database& db, const litesql::Record& rec);
    ProcessUnit(const ProcessUnit& obj);
    const ProcessUnit& operator=(const ProcessUnit& obj);
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
    std::auto_ptr<ProcessUnit> upcast();
    std::auto_ptr<ProcessUnit> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, ProcessUnit o);
class HiveDb : public litesql::Database {
public:
    HiveDb(std::string backendType, std::string connInfo);
protected:
    virtual std::vector<litesql::Database::SchemaItem> getSchema() const;
    static void initialize();
};
}
#endif
