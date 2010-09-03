#ifndef hivedb_hpp
#define hivedb_hpp
#include "litesql.hpp"
namespace orm {
class MediaFile;
class MetaData;
class Project;
class Job;
class JobLog;
class Preset;
class CodecPreset;
class Filter;
class FilterParameter;
class Format;
class FormatParameter;
class Stream;
class ProcessUnit;
class MediaFileMetaDataRelationMediaFile2MetaData {
public:
    class Row {
    public:
        litesql::Field<int> metaData;
        litesql::Field<int> mediaFile;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType MediaFile;
    static const litesql::FieldType MetaData;
    static void link(const litesql::Database& db, const orm::MediaFile& o0, const orm::MetaData& o1);
    static void unlink(const litesql::Database& db, const orm::MediaFile& o0, const orm::MetaData& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<MediaFileMetaDataRelationMediaFile2MetaData::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class MediaFileProjectRelationProject2OutMediaFile {
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
    static void link(const litesql::Database& db, const orm::MediaFile& o0, const orm::Project& o1);
    static void unlink(const litesql::Database& db, const orm::MediaFile& o0, const orm::Project& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<MediaFileProjectRelationProject2OutMediaFile::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class MediaFileProjectRelationProject2InMediaFile {
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
    static void link(const litesql::Database& db, const orm::MediaFile& o0, const orm::Project& o1);
    static void unlink(const litesql::Database& db, const orm::MediaFile& o0, const orm::Project& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<MediaFileProjectRelationProject2InMediaFile::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class JobProjectRelationProject2Job {
public:
    class Row {
    public:
        litesql::Field<int> project;
        litesql::Field<int> job;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Job;
    static const litesql::FieldType Project;
    static void link(const litesql::Database& db, const orm::Job& o0, const orm::Project& o1);
    static void unlink(const litesql::Database& db, const orm::Job& o0, const orm::Project& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<JobProjectRelationProject2Job::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class JobJobLogRelationJob2JobLog {
public:
    class Row {
    public:
        litesql::Field<int> jobLog;
        litesql::Field<int> job;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Job;
    static const litesql::FieldType JobLog;
    static void link(const litesql::Database& db, const orm::Job& o0, const orm::JobLog& o1);
    static void unlink(const litesql::Database& db, const orm::Job& o0, const orm::JobLog& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<JobJobLogRelationJob2JobLog::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class PresetProjectRelationProject2Preset {
public:
    class Row {
    public:
        litesql::Field<int> project;
        litesql::Field<int> preset;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Preset;
    static const litesql::FieldType Project;
    static void link(const litesql::Database& db, const orm::Preset& o0, const orm::Project& o1);
    static void unlink(const litesql::Database& db, const orm::Preset& o0, const orm::Project& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<PresetProjectRelationProject2Preset::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class CodecPresetPresetRelationPreset2CodecPreset {
public:
    class Row {
    public:
        litesql::Field<int> preset;
        litesql::Field<int> codecPreset;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType CodecPreset;
    static const litesql::FieldType Preset;
    static void link(const litesql::Database& db, const orm::CodecPreset& o0, const orm::Preset& o1);
    static void unlink(const litesql::Database& db, const orm::CodecPreset& o0, const orm::Preset& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<CodecPresetPresetRelationPreset2CodecPreset::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class FilterPresetRelationPreset2Filter {
public:
    class Row {
    public:
        litesql::Field<int> preset;
        litesql::Field<int> filter;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Filter;
    static const litesql::FieldType Preset;
    static void link(const litesql::Database& db, const orm::Filter& o0, const orm::Preset& o1);
    static void unlink(const litesql::Database& db, const orm::Filter& o0, const orm::Preset& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<FilterPresetRelationPreset2Filter::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class FilterFilterParameterRelationFilter2FilterParameter {
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
    static void link(const litesql::Database& db, const orm::Filter& o0, const orm::FilterParameter& o1);
    static void unlink(const litesql::Database& db, const orm::Filter& o0, const orm::FilterParameter& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<FilterFilterParameterRelationFilter2FilterParameter::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class FormatPresetRelationPreset2Format {
public:
    class Row {
    public:
        litesql::Field<int> preset;
        litesql::Field<int> format;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Format;
    static const litesql::FieldType Preset;
    static void link(const litesql::Database& db, const orm::Format& o0, const orm::Preset& o1);
    static void unlink(const litesql::Database& db, const orm::Format& o0, const orm::Preset& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<FormatPresetRelationPreset2Format::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class FormatFormatParameterRelationFormat2FormatParameter {
public:
    class Row {
    public:
        litesql::Field<int> formatParameter;
        litesql::Field<int> format;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Format;
    static const litesql::FieldType FormatParameter;
    static void link(const litesql::Database& db, const orm::Format& o0, const orm::FormatParameter& o1);
    static void unlink(const litesql::Database& db, const orm::Format& o0, const orm::FormatParameter& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<FormatFormatParameterRelationFormat2FormatParameter::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class MediaFileStreamRelationMediaFile2Stream {
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
    static void link(const litesql::Database& db, const orm::MediaFile& o0, const orm::Stream& o1);
    static void unlink(const litesql::Database& db, const orm::MediaFile& o0, const orm::Stream& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<MediaFileStreamRelationMediaFile2Stream::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class FormatStreamRelationStream2Format {
public:
    class Row {
    public:
        litesql::Field<int> stream;
        litesql::Field<int> format;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Format;
    static const litesql::FieldType Stream;
    static void link(const litesql::Database& db, const orm::Format& o0, const orm::Stream& o1);
    static void unlink(const litesql::Database& db, const orm::Format& o0, const orm::Stream& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<FormatStreamRelationStream2Format::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class ProcessUnitStreamRelationStream2ProcessUnit {
public:
    class Row {
    public:
        litesql::Field<int> stream;
        litesql::Field<int> processUnit;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType ProcessUnit;
    static const litesql::FieldType Stream;
    static void link(const litesql::Database& db, const orm::ProcessUnit& o0, const orm::Stream& o1);
    static void unlink(const litesql::Database& db, const orm::ProcessUnit& o0, const orm::Stream& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<ProcessUnitStreamRelationStream2ProcessUnit::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class MediaFile : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class MetadataHandle : public litesql::RelationHandle<MediaFile> {
    public:
        MetadataHandle(const MediaFile& owner);
        void link(const MetaData& o0);
        void unlink(const MetaData& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<MetaData> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<MediaFileMetaDataRelationMediaFile2MetaData::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class ProjectoutfileHandle : public litesql::RelationHandle<MediaFile> {
    public:
        ProjectoutfileHandle(const MediaFile& owner);
        void link(const Project& o0);
        void unlink(const Project& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Project> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<MediaFileProjectRelationProject2OutMediaFile::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class ProjectinfileHandle : public litesql::RelationHandle<MediaFile> {
    public:
        ProjectinfileHandle(const MediaFile& owner);
        void link(const Project& o0);
        void unlink(const Project& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Project> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<MediaFileProjectRelationProject2InMediaFile::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class StreamsHandle : public litesql::RelationHandle<MediaFile> {
    public:
        StreamsHandle(const MediaFile& owner);
        void link(const Stream& o0);
        void unlink(const Stream& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Stream> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<MediaFileStreamRelationMediaFile2Stream::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Filepath;
    litesql::Field<std::string> filepath;
    static const litesql::FieldType Filename;
    litesql::Field<std::string> filename;
    static const litesql::FieldType Filesize;
    litesql::Field<double> filesize;
    static const litesql::FieldType Streamcount;
    litesql::Field<int> streamcount;
    static const litesql::FieldType Containertype;
    litesql::Field<std::string> containertype;
    static const litesql::FieldType Duration;
    litesql::Field<double> duration;
    static const litesql::FieldType Starttime;
    litesql::Field<double> starttime;
    static const litesql::FieldType Bitrate;
    litesql::Field<int> bitrate;
    static const litesql::FieldType Filetype;
    litesql::Field<int> filetype;
    static const litesql::FieldType Parent;
    litesql::Field<int> parent;
    static const litesql::FieldType Created;
    litesql::Field<litesql::DateTime> created;
protected:
    void defaults();
public:
    MediaFile(const litesql::Database& db);
    MediaFile(const litesql::Database& db, const litesql::Record& rec);
    MediaFile(const MediaFile& obj);
    const MediaFile& operator=(const MediaFile& obj);
    MediaFile::MetadataHandle metadata();
    MediaFile::ProjectoutfileHandle projectoutfile();
    MediaFile::ProjectinfileHandle projectinfile();
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
class MetaData : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class MediafileHandle : public litesql::RelationHandle<MetaData> {
    public:
        MediafileHandle(const MetaData& owner);
        void link(const MediaFile& o0);
        void unlink(const MediaFile& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<MediaFile> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<MediaFileMetaDataRelationMediaFile2MetaData::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Metakey;
    litesql::Field<std::string> metakey;
    static const litesql::FieldType Metavalue;
    litesql::Field<std::string> metavalue;
protected:
    void defaults();
public:
    MetaData(const litesql::Database& db);
    MetaData(const litesql::Database& db, const litesql::Record& rec);
    MetaData(const MetaData& obj);
    const MetaData& operator=(const MetaData& obj);
    MetaData::MediafileHandle mediafile();
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
    std::auto_ptr<MetaData> upcast();
    std::auto_ptr<MetaData> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, MetaData o);
class Project : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class OutmediafilesHandle : public litesql::RelationHandle<Project> {
    public:
        OutmediafilesHandle(const Project& owner);
        void link(const MediaFile& o0);
        void unlink(const MediaFile& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<MediaFile> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<MediaFileProjectRelationProject2OutMediaFile::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class InmediafilesHandle : public litesql::RelationHandle<Project> {
    public:
        InmediafilesHandle(const Project& owner);
        void link(const MediaFile& o0);
        void unlink(const MediaFile& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<MediaFile> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<MediaFileProjectRelationProject2InMediaFile::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class JobsHandle : public litesql::RelationHandle<Project> {
    public:
        JobsHandle(const Project& owner);
        void link(const Job& o0);
        void unlink(const Job& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Job> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<JobProjectRelationProject2Job::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class PresetsHandle : public litesql::RelationHandle<Project> {
    public:
        PresetsHandle(const Project& owner);
        void link(const Preset& o0);
        void unlink(const Preset& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Preset> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<PresetProjectRelationProject2Preset::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    Project::OutmediafilesHandle outmediafiles();
    Project::InmediafilesHandle inmediafiles();
    Project::JobsHandle jobs();
    Project::PresetsHandle presets();
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
class Job : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class ProjectHandle : public litesql::RelationHandle<Job> {
    public:
        ProjectHandle(const Job& owner);
        void link(const Project& o0);
        void unlink(const Project& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Project> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<JobProjectRelationProject2Job::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class LogsHandle : public litesql::RelationHandle<Job> {
    public:
        LogsHandle(const Job& owner);
        void link(const JobLog& o0);
        void unlink(const JobLog& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<JobLog> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<JobJobLogRelationJob2JobLog::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Created;
    litesql::Field<litesql::DateTime> created;
    static const litesql::FieldType Begintime;
    litesql::Field<litesql::DateTime> begintime;
    static const litesql::FieldType Endtime;
    litesql::Field<litesql::DateTime> endtime;
    static const litesql::FieldType Status;
    litesql::Field<std::string> status;
    static const litesql::FieldType Infile;
    litesql::Field<std::string> infile;
    static const litesql::FieldType Outfile;
    litesql::Field<std::string> outfile;
    static const litesql::FieldType Starttime;
    litesql::Field<double> starttime;
    static const litesql::FieldType Duration;
    litesql::Field<double> duration;
    static const litesql::FieldType Progress;
    litesql::Field<int> progress;
protected:
    void defaults();
public:
    Job(const litesql::Database& db);
    Job(const litesql::Database& db, const litesql::Record& rec);
    Job(const Job& obj);
    const Job& operator=(const Job& obj);
    Job::ProjectHandle project();
    Job::LogsHandle logs();
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
class JobLog : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class JobHandle : public litesql::RelationHandle<JobLog> {
    public:
        JobHandle(const JobLog& owner);
        void link(const Job& o0);
        void unlink(const Job& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Job> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<JobJobLogRelationJob2JobLog::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Created;
    litesql::Field<litesql::DateTime> created;
    static const litesql::FieldType Messagetype;
    litesql::Field<int> messagetype;
    static const litesql::FieldType Message;
    litesql::Field<std::string> message;
protected:
    void defaults();
public:
    JobLog(const litesql::Database& db);
    JobLog(const litesql::Database& db, const litesql::Record& rec);
    JobLog(const JobLog& obj);
    const JobLog& operator=(const JobLog& obj);
    JobLog::JobHandle job();
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
    std::auto_ptr<JobLog> upcast();
    std::auto_ptr<JobLog> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, JobLog o);
class Preset : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class ProjectHandle : public litesql::RelationHandle<Preset> {
    public:
        ProjectHandle(const Preset& owner);
        void link(const Project& o0);
        void unlink(const Project& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Project> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<PresetProjectRelationProject2Preset::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class CodecpresetsHandle : public litesql::RelationHandle<Preset> {
    public:
        CodecpresetsHandle(const Preset& owner);
        void link(const CodecPreset& o0);
        void unlink(const CodecPreset& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<CodecPreset> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<CodecPresetPresetRelationPreset2CodecPreset::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class FiltersHandle : public litesql::RelationHandle<Preset> {
    public:
        FiltersHandle(const Preset& owner);
        void link(const Filter& o0);
        void unlink(const Filter& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Filter> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FilterPresetRelationPreset2Filter::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class FormatHandle : public litesql::RelationHandle<Preset> {
    public:
        FormatHandle(const Preset& owner);
        void link(const Format& o0);
        void unlink(const Format& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Format> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FormatPresetRelationPreset2Format::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Presetname;
    litesql::Field<std::string> presetname;
    static const litesql::FieldType Description;
    litesql::Field<std::string> description;
    static const litesql::FieldType Presettype;
    litesql::Field<int> presettype;
    static const litesql::FieldType Created;
    litesql::Field<litesql::DateTime> created;
protected:
    void defaults();
public:
    Preset(const litesql::Database& db);
    Preset(const litesql::Database& db, const litesql::Record& rec);
    Preset(const Preset& obj);
    const Preset& operator=(const Preset& obj);
    Preset::ProjectHandle project();
    Preset::CodecpresetsHandle codecpresets();
    Preset::FiltersHandle filters();
    Preset::FormatHandle format();
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
    std::auto_ptr<Preset> upcast();
    std::auto_ptr<Preset> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, Preset o);
class CodecPreset : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class PresetHandle : public litesql::RelationHandle<CodecPreset> {
    public:
        PresetHandle(const CodecPreset& owner);
        void link(const Preset& o0);
        void unlink(const Preset& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Preset> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<CodecPresetPresetRelationPreset2CodecPreset::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Presetname;
    litesql::Field<std::string> presetname;
    static const litesql::FieldType Description;
    litesql::Field<std::string> description;
    static const litesql::FieldType Presettype;
    litesql::Field<int> presettype;
    static const litesql::FieldType Created;
    litesql::Field<litesql::DateTime> created;
protected:
    void defaults();
public:
    CodecPreset(const litesql::Database& db);
    CodecPreset(const litesql::Database& db, const litesql::Record& rec);
    CodecPreset(const CodecPreset& obj);
    const CodecPreset& operator=(const CodecPreset& obj);
    CodecPreset::PresetHandle preset();
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
class Filter : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class PresetHandle : public litesql::RelationHandle<Filter> {
    public:
        PresetHandle(const Filter& owner);
        void link(const Preset& o0);
        void unlink(const Preset& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Preset> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FilterPresetRelationPreset2Filter::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class ParameterHandle : public litesql::RelationHandle<Filter> {
    public:
        ParameterHandle(const Filter& owner);
        void link(const FilterParameter& o0);
        void unlink(const FilterParameter& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<FilterParameter> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FilterFilterParameterRelationFilter2FilterParameter::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Filterid;
    litesql::Field<std::string> filterid;
    static const litesql::FieldType Filtername;
    litesql::Field<std::string> filtername;
protected:
    void defaults();
public:
    Filter(const litesql::Database& db);
    Filter(const litesql::Database& db, const litesql::Record& rec);
    Filter(const Filter& obj);
    const Filter& operator=(const Filter& obj);
    Filter::PresetHandle preset();
    Filter::ParameterHandle parameter();
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
        litesql::DataSource<FilterFilterParameterRelationFilter2FilterParameter::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
class Format : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class PresetHandle : public litesql::RelationHandle<Format> {
    public:
        PresetHandle(const Format& owner);
        void link(const Preset& o0);
        void unlink(const Preset& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Preset> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FormatPresetRelationPreset2Format::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class ParameterHandle : public litesql::RelationHandle<Format> {
    public:
        ParameterHandle(const Format& owner);
        void link(const FormatParameter& o0);
        void unlink(const FormatParameter& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<FormatParameter> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FormatFormatParameterRelationFormat2FormatParameter::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class StreamHandle : public litesql::RelationHandle<Format> {
    public:
        StreamHandle(const Format& owner);
        void link(const Stream& o0);
        void unlink(const Stream& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Stream> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FormatStreamRelationStream2Format::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Formattype;
    litesql::Field<int> formattype;
    static const litesql::FieldType Formatid;
    litesql::Field<int> formatid;
    static const litesql::FieldType Mimetype;
    litesql::Field<std::string> mimetype;
protected:
    void defaults();
public:
    Format(const litesql::Database& db);
    Format(const litesql::Database& db, const litesql::Record& rec);
    Format(const Format& obj);
    const Format& operator=(const Format& obj);
    Format::PresetHandle preset();
    Format::ParameterHandle parameter();
    Format::StreamHandle stream();
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
    std::auto_ptr<Format> upcast();
    std::auto_ptr<Format> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, Format o);
class FormatParameter : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class FormatHandle : public litesql::RelationHandle<FormatParameter> {
    public:
        FormatHandle(const FormatParameter& owner);
        void link(const Format& o0);
        void unlink(const Format& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Format> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FormatFormatParameterRelationFormat2FormatParameter::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Paramkey;
    litesql::Field<std::string> paramkey;
    static const litesql::FieldType Paramvalue;
    litesql::Field<std::string> paramvalue;
protected:
    void defaults();
public:
    FormatParameter(const litesql::Database& db);
    FormatParameter(const litesql::Database& db, const litesql::Record& rec);
    FormatParameter(const FormatParameter& obj);
    const FormatParameter& operator=(const FormatParameter& obj);
    FormatParameter::FormatHandle format();
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
    std::auto_ptr<FormatParameter> upcast();
    std::auto_ptr<FormatParameter> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, FormatParameter o);
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
        litesql::DataSource<MediaFileStreamRelationMediaFile2Stream::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class FormatHandle : public litesql::RelationHandle<Stream> {
    public:
        FormatHandle(const Stream& owner);
        void link(const Format& o0);
        void unlink(const Format& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Format> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FormatStreamRelationStream2Format::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class ProcessunitsHandle : public litesql::RelationHandle<Stream> {
    public:
        ProcessunitsHandle(const Stream& owner);
        void link(const ProcessUnit& o0);
        void unlink(const ProcessUnit& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<ProcessUnit> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<ProcessUnitStreamRelationStream2ProcessUnit::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Streamtype;
    litesql::Field<int> streamtype;
    static const litesql::FieldType Streamindex;
    litesql::Field<int> streamindex;
    static const litesql::FieldType Duration;
    litesql::Field<double> duration;
protected:
    void defaults();
public:
    Stream(const litesql::Database& db);
    Stream(const litesql::Database& db, const litesql::Record& rec);
    Stream(const Stream& obj);
    const Stream& operator=(const Stream& obj);
    Stream::MediafileHandle mediafile();
    Stream::FormatHandle format();
    Stream::ProcessunitsHandle processunits();
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
class ProcessUnit : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class StreamHandle : public litesql::RelationHandle<ProcessUnit> {
    public:
        StreamHandle(const ProcessUnit& owner);
        void link(const Stream& o0);
        void unlink(const Stream& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Stream> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<ProcessUnitStreamRelationStream2ProcessUnit::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
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
    ProcessUnit::StreamHandle stream();
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
