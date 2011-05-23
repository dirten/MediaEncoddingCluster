#ifndef hivedb_hpp
#define hivedb_hpp
#include "litesql.hpp"
namespace db {
class Project;
class Filter;
class FilterParameter;
class MediaFile;
class ProfileGroup;
class Profile;
class Preset;
class ProfileParameter;
class Stream;
class StreamParameter;
class CodecPreset;
class CodecPresetParameter;
class Config;
class Job;
class JobLog;
class JobDetail;
class Watchfolder;
class ProcessUnit;
class User;
class UserGroup;
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
class PresetProjectRelation {
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
    static void link(const litesql::Database& db, const db::Preset& o0, const db::Project& o1);
    static void unlink(const litesql::Database& db, const db::Preset& o0, const db::Project& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<PresetProjectRelation::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class PresetUserRelation {
public:
    class Row {
    public:
        litesql::Field<int> user;
        litesql::Field<int> preset;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Preset;
    static const litesql::FieldType User;
    static void link(const litesql::Database& db, const db::Preset& o0, const db::User& o1);
    static void unlink(const litesql::Database& db, const db::Preset& o0, const db::User& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<PresetUserRelation::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class FilterProfileRelation {
public:
    class Row {
    public:
        litesql::Field<int> profile;
        litesql::Field<int> filter;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Filter;
    static const litesql::FieldType Profile;
    static void link(const litesql::Database& db, const db::Filter& o0, const db::Profile& o1);
    static void unlink(const litesql::Database& db, const db::Filter& o0, const db::Profile& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<FilterProfileRelation::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class ProfileGroup2Profile {
public:
    class Row {
    public:
        litesql::Field<int> profileGroup;
        litesql::Field<int> profile;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Profile;
    static const litesql::FieldType ProfileGroup;
    static void link(const litesql::Database& db, const db::Profile& o0, const db::ProfileGroup& o1);
    static void unlink(const litesql::Database& db, const db::Profile& o0, const db::ProfileGroup& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<ProfileGroup2Profile::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren {
public:
    class Row {
    public:
        litesql::Field<int> profileGroup2;
        litesql::Field<int> profileGroup1;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType ProfileGroup1;
    static const litesql::FieldType ProfileGroup2;
    static void link(const litesql::Database& db, const db::ProfileGroup& o0, const db::ProfileGroup& o1);
    static void unlink(const litesql::Database& db, const db::ProfileGroup& o0, const db::ProfileGroup& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<db::ProfileGroup> getProfileGroup1(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
    static litesql::DataSource<db::ProfileGroup> getProfileGroup2(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
};
class ProfileProfileParameterRelation {
public:
    class Row {
    public:
        litesql::Field<int> profileParameter;
        litesql::Field<int> profile;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Profile;
    static const litesql::FieldType ProfileParameter;
    static void link(const litesql::Database& db, const db::Profile& o0, const db::ProfileParameter& o1);
    static void unlink(const litesql::Database& db, const db::Profile& o0, const db::ProfileParameter& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<ProfileProfileParameterRelation::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
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
class StreamStreamParameterRelation {
public:
    class Row {
    public:
        litesql::Field<int> streamParameter;
        litesql::Field<int> stream;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Stream;
    static const litesql::FieldType StreamParameter;
    static void link(const litesql::Database& db, const db::Stream& o0, const db::StreamParameter& o1);
    static void unlink(const litesql::Database& db, const db::Stream& o0, const db::StreamParameter& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<StreamStreamParameterRelation::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class FilterStreamRelation {
public:
    class Row {
    public:
        litesql::Field<int> stream;
        litesql::Field<int> filter;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Filter;
    static const litesql::FieldType Stream;
    static void link(const litesql::Database& db, const db::Filter& o0, const db::Stream& o1);
    static void unlink(const litesql::Database& db, const db::Filter& o0, const db::Stream& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<FilterStreamRelation::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
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
class CodecPresetCodecPresetParameterRelationCodecPreset2Parameter {
public:
    class Row {
    public:
        litesql::Field<int> codecPresetParameter;
        litesql::Field<int> codecPreset;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType CodecPreset;
    static const litesql::FieldType CodecPresetParameter;
    static void link(const litesql::Database& db, const db::CodecPreset& o0, const db::CodecPresetParameter& o1);
    static void unlink(const litesql::Database& db, const db::CodecPreset& o0, const db::CodecPresetParameter& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class CodecPresetProfileRelationVideoCodecPreset2Profile {
public:
    class Row {
    public:
        litesql::Field<int> profile;
        litesql::Field<int> codecPreset;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType CodecPreset;
    static const litesql::FieldType Profile;
    static void link(const litesql::Database& db, const db::CodecPreset& o0, const db::Profile& o1);
    static void unlink(const litesql::Database& db, const db::CodecPreset& o0, const db::Profile& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<CodecPresetProfileRelationVideoCodecPreset2Profile::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class CodecPresetProfileRelationAudioCodecPreset2Profile {
public:
    class Row {
    public:
        litesql::Field<int> profile;
        litesql::Field<int> codecPreset;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType CodecPreset;
    static const litesql::FieldType Profile;
    static void link(const litesql::Database& db, const db::CodecPreset& o0, const db::Profile& o1);
    static void unlink(const litesql::Database& db, const db::CodecPreset& o0, const db::Profile& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<CodecPresetProfileRelationAudioCodecPreset2Profile::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class JobJobLogRelationJobJobLog {
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
    static void link(const litesql::Database& db, const db::Job& o0, const db::JobLog& o1);
    static void unlink(const litesql::Database& db, const db::Job& o0, const db::JobLog& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<JobJobLogRelationJobJobLog::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
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
class JobPresetRelation {
public:
    class Row {
    public:
        litesql::Field<int> preset;
        litesql::Field<int> job;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Job;
    static const litesql::FieldType Preset;
    static void link(const litesql::Database& db, const db::Job& o0, const db::Preset& o1);
    static void unlink(const litesql::Database& db, const db::Job& o0, const db::Preset& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<JobPresetRelation::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
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
class UserUserGroupRelationUser2UserGroup {
public:
    class Row {
    public:
        litesql::Field<int> userGroup;
        litesql::Field<int> user;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType User;
    static const litesql::FieldType UserGroup;
    static void link(const litesql::Database& db, const db::User& o0, const db::UserGroup& o1);
    static void unlink(const litesql::Database& db, const db::User& o0, const db::UserGroup& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<UserUserGroupRelationUser2UserGroup::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
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
    class PresetsHandle : public litesql::RelationHandle<Project> {
    public:
        PresetsHandle(const Project& owner);
        void link(const Preset& o0);
        void unlink(const Preset& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Preset> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<PresetProjectRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    Project::PresetsHandle presets();
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
    class ProfileHandle : public litesql::RelationHandle<Filter> {
    public:
        ProfileHandle(const Filter& owner);
        void link(const Profile& o0);
        void unlink(const Profile& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Profile> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FilterProfileRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class StreamHandle : public litesql::RelationHandle<Filter> {
    public:
        StreamHandle(const Filter& owner);
        void link(const Stream& o0);
        void unlink(const Stream& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Stream> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FilterStreamRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    Filter::ProfileHandle profile();
    Filter::StreamHandle stream();
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
    static const litesql::FieldType Starttime;
    litesql::Field<double> starttime;
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
class ProfileGroup : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class ProfilesHandle : public litesql::RelationHandle<ProfileGroup> {
    public:
        ProfilesHandle(const ProfileGroup& owner);
        void link(const Profile& o0);
        void unlink(const Profile& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Profile> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<ProfileGroup2Profile::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class ChildrensHandle : public litesql::RelationHandle<ProfileGroup> {
    public:
        ChildrensHandle(const ProfileGroup& owner);
        void link(const ProfileGroup& o0);
        void unlink(const ProfileGroup& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<ProfileGroup> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class ParentHandle : public litesql::RelationHandle<ProfileGroup> {
    public:
        ParentHandle(const ProfileGroup& owner);
        void link(const ProfileGroup& o0);
        void unlink(const ProfileGroup& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<ProfileGroup> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
protected:
    void defaults();
public:
    ProfileGroup(const litesql::Database& db);
    ProfileGroup(const litesql::Database& db, const litesql::Record& rec);
    ProfileGroup(const ProfileGroup& obj);
    const ProfileGroup& operator=(const ProfileGroup& obj);
    ProfileGroup::ProfilesHandle profiles();
    ProfileGroup::ChildrensHandle childrens();
    ProfileGroup::ParentHandle parent();
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
    std::auto_ptr<ProfileGroup> upcast();
    std::auto_ptr<ProfileGroup> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, ProfileGroup o);
class Profile : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class FilterHandle : public litesql::RelationHandle<Profile> {
    public:
        FilterHandle(const Profile& owner);
        void link(const Filter& o0);
        void unlink(const Filter& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Filter> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FilterProfileRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class GroupHandle : public litesql::RelationHandle<Profile> {
    public:
        GroupHandle(const Profile& owner);
        void link(const ProfileGroup& o0);
        void unlink(const ProfileGroup& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<ProfileGroup> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<ProfileGroup2Profile::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class ParamsHandle : public litesql::RelationHandle<Profile> {
    public:
        ParamsHandle(const Profile& owner);
        void link(const ProfileParameter& o0);
        void unlink(const ProfileParameter& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<ProfileParameter> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<ProfileProfileParameterRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    class VpresetHandle : public litesql::RelationHandle<Profile> {
    public:
        VpresetHandle(const Profile& owner);
        void link(const CodecPreset& o0);
        void unlink(const CodecPreset& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<CodecPreset> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<CodecPresetProfileRelationVideoCodecPreset2Profile::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class ApresetHandle : public litesql::RelationHandle<Profile> {
    public:
        ApresetHandle(const Profile& owner);
        void link(const CodecPreset& o0);
        void unlink(const CodecPreset& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<CodecPreset> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<CodecPresetProfileRelationAudioCodecPreset2Profile::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    static const litesql::FieldType Deinterlace;
    litesql::Field<int> deinterlace;
protected:
    void defaults();
public:
    Profile(const litesql::Database& db);
    Profile(const litesql::Database& db, const litesql::Record& rec);
    Profile(const Profile& obj);
    const Profile& operator=(const Profile& obj);
    Profile::FilterHandle filter();
    Profile::GroupHandle group();
    Profile::ParamsHandle params();
    Profile::ProjectHandle project();
    Profile::VpresetHandle vpreset();
    Profile::ApresetHandle apreset();
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
        litesql::DataSource<PresetProjectRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class UserHandle : public litesql::RelationHandle<Preset> {
    public:
        UserHandle(const Preset& owner);
        void link(const User& o0);
        void unlink(const User& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<User> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<PresetUserRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class JobHandle : public litesql::RelationHandle<Preset> {
    public:
        JobHandle(const Preset& owner);
        void link(const Job& o0);
        void unlink(const Job& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Job> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<JobPresetRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Uuid;
    litesql::Field<std::string> uuid;
    static const litesql::FieldType Name;
    litesql::Field<std::string> name;
    static const litesql::FieldType Filename;
    litesql::Field<std::string> filename;
    static const litesql::FieldType Data;
    litesql::Field<std::string> data;
protected:
    void defaults();
public:
    Preset(const litesql::Database& db);
    Preset(const litesql::Database& db, const litesql::Record& rec);
    Preset(const Preset& obj);
    const Preset& operator=(const Preset& obj);
    Preset::ProjectHandle project();
    Preset::UserHandle user();
    Preset::JobHandle job();
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
class ProfileParameter : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class ProfileHandle : public litesql::RelationHandle<ProfileParameter> {
    public:
        ProfileHandle(const ProfileParameter& owner);
        void link(const Profile& o0);
        void unlink(const Profile& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Profile> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<ProfileProfileParameterRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    static const litesql::FieldType Val;
    litesql::Field<std::string> val;
    static const litesql::FieldType Mediatype;
    litesql::Field<int> mediatype;
protected:
    void defaults();
public:
    ProfileParameter(const litesql::Database& db);
    ProfileParameter(const litesql::Database& db, const litesql::Record& rec);
    ProfileParameter(const ProfileParameter& obj);
    const ProfileParameter& operator=(const ProfileParameter& obj);
    ProfileParameter::ProfileHandle profile();
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
    std::auto_ptr<ProfileParameter> upcast();
    std::auto_ptr<ProfileParameter> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, ProfileParameter o);
class Stream : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class ParamsHandle : public litesql::RelationHandle<Stream> {
    public:
        ParamsHandle(const Stream& owner);
        void link(const StreamParameter& o0);
        void unlink(const StreamParameter& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<StreamParameter> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<StreamStreamParameterRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class FiltersHandle : public litesql::RelationHandle<Stream> {
    public:
        FiltersHandle(const Stream& owner);
        void link(const Filter& o0);
        void unlink(const Filter& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Filter> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<FilterStreamRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    litesql::Field<litesql::Blob> extradata;
    static const litesql::FieldType Aspectratio;
    litesql::Field<std::string> aspectratio;
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
    Stream::ParamsHandle params();
    Stream::FiltersHandle filters();
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
class StreamParameter : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class StreamHandle : public litesql::RelationHandle<StreamParameter> {
    public:
        StreamHandle(const StreamParameter& owner);
        void link(const Stream& o0);
        void unlink(const Stream& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Stream> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<StreamStreamParameterRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    static const litesql::FieldType Val;
    litesql::Field<std::string> val;
protected:
    void defaults();
public:
    StreamParameter(const litesql::Database& db);
    StreamParameter(const litesql::Database& db, const litesql::Record& rec);
    StreamParameter(const StreamParameter& obj);
    const StreamParameter& operator=(const StreamParameter& obj);
    StreamParameter::StreamHandle stream();
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
    std::auto_ptr<StreamParameter> upcast();
    std::auto_ptr<StreamParameter> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, StreamParameter o);
class CodecPreset : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class ParamsHandle : public litesql::RelationHandle<CodecPreset> {
    public:
        ParamsHandle(const CodecPreset& owner);
        void link(const CodecPresetParameter& o0);
        void unlink(const CodecPresetParameter& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<CodecPresetParameter> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class VprofileHandle : public litesql::RelationHandle<CodecPreset> {
    public:
        VprofileHandle(const CodecPreset& owner);
        void link(const Profile& o0);
        void unlink(const Profile& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Profile> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<CodecPresetProfileRelationVideoCodecPreset2Profile::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class AprofileHandle : public litesql::RelationHandle<CodecPreset> {
    public:
        AprofileHandle(const CodecPreset& owner);
        void link(const Profile& o0);
        void unlink(const Profile& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Profile> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<CodecPresetProfileRelationAudioCodecPreset2Profile::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    CodecPreset::ParamsHandle params();
    CodecPreset::VprofileHandle vprofile();
    CodecPreset::AprofileHandle aprofile();
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
class CodecPresetParameter : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class PresetHandle : public litesql::RelationHandle<CodecPresetParameter> {
    public:
        PresetHandle(const CodecPresetParameter& owner);
        void link(const CodecPreset& o0);
        void unlink(const CodecPreset& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<CodecPreset> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    static const litesql::FieldType Val;
    litesql::Field<std::string> val;
protected:
    void defaults();
public:
    CodecPresetParameter(const litesql::Database& db);
    CodecPresetParameter(const litesql::Database& db, const litesql::Record& rec);
    CodecPresetParameter(const CodecPresetParameter& obj);
    const CodecPresetParameter& operator=(const CodecPresetParameter& obj);
    CodecPresetParameter::PresetHandle preset();
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
    std::auto_ptr<CodecPresetParameter> upcast();
    std::auto_ptr<CodecPresetParameter> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, CodecPresetParameter o);
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
    class JoblogHandle : public litesql::RelationHandle<Job> {
    public:
        JoblogHandle(const Job& owner);
        void link(const JobLog& o0);
        void unlink(const JobLog& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<JobLog> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<JobJobLogRelationJobJobLog::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    class PresetHandle : public litesql::RelationHandle<Job> {
    public:
        PresetHandle(const Job& owner);
        void link(const Preset& o0);
        void unlink(const Preset& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Preset> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<JobPresetRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    static const litesql::FieldType Uuid;
    litesql::Field<std::string> uuid;
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
    static const litesql::FieldType Data;
    litesql::Field<std::string> data;
protected:
    void defaults();
public:
    Job(const litesql::Database& db);
    Job(const litesql::Database& db, const litesql::Record& rec);
    Job(const Job& obj);
    const Job& operator=(const Job& obj);
    Job::JoblogHandle joblog();
    Job::InputfileHandle inputfile();
    Job::OutputfileHandle outputfile();
    Job::PresetHandle preset();
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
class JobLog : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class LogjobHandle : public litesql::RelationHandle<JobLog> {
    public:
        LogjobHandle(const JobLog& owner);
        void link(const Job& o0);
        void unlink(const Job& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Job> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<JobJobLogRelationJobJobLog::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    static const litesql::FieldType Message;
    litesql::Field<std::string> message;
protected:
    void defaults();
public:
    JobLog(const litesql::Database& db);
    JobLog(const litesql::Database& db, const litesql::Record& rec);
    JobLog(const JobLog& obj);
    const JobLog& operator=(const JobLog& obj);
    JobLog::LogjobHandle logjob();
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
    static const litesql::FieldType Deinterlace;
    litesql::Field<int> deinterlace;
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
    static const litesql::FieldType Timebasenum;
    litesql::Field<int> timebasenum;
    static const litesql::FieldType Timebaseden;
    litesql::Field<int> timebaseden;
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
class User : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class PresetsHandle : public litesql::RelationHandle<User> {
    public:
        PresetsHandle(const User& owner);
        void link(const Preset& o0);
        void unlink(const Preset& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Preset> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<PresetUserRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class UserGroupHandle : public litesql::RelationHandle<User> {
    public:
        UserGroupHandle(const User& owner);
        void link(const UserGroup& o0);
        void unlink(const UserGroup& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<UserGroup> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<UserUserGroupRelationUser2UserGroup::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Authname;
    litesql::Field<std::string> authname;
    static const litesql::FieldType Authpass;
    litesql::Field<std::string> authpass;
    static const litesql::FieldType Company;
    litesql::Field<std::string> company;
    static const litesql::FieldType Firstname;
    litesql::Field<std::string> firstname;
    static const litesql::FieldType Lastname;
    litesql::Field<std::string> lastname;
    static const litesql::FieldType Street;
    litesql::Field<std::string> street;
    static const litesql::FieldType City;
    litesql::Field<std::string> city;
    static const litesql::FieldType Zip;
    litesql::Field<std::string> zip;
    static const litesql::FieldType Country;
    litesql::Field<std::string> country;
    static const litesql::FieldType State;
    litesql::Field<std::string> state;
    static const litesql::FieldType Telefone;
    litesql::Field<std::string> telefone;
    static const litesql::FieldType Fax;
    litesql::Field<std::string> fax;
    static const litesql::FieldType Email;
    litesql::Field<std::string> email;
    static const litesql::FieldType Www;
    litesql::Field<std::string> www;
    static const litesql::FieldType Language;
    litesql::Field<std::string> language;
    static const litesql::FieldType Licensekey;
    litesql::Field<std::string> licensekey;
    static const litesql::FieldType Apikey;
    litesql::Field<std::string> apikey;
    static const litesql::FieldType Registered;
    litesql::Field<litesql::Date> registered;
    static const litesql::FieldType Fileroot;
    litesql::Field<std::string> fileroot;
protected:
    void defaults();
public:
    User(const litesql::Database& db);
    User(const litesql::Database& db, const litesql::Record& rec);
    User(const User& obj);
    const User& operator=(const User& obj);
    User::PresetsHandle presets();
    User::UserGroupHandle userGroup();
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
    std::auto_ptr<User> upcast();
    std::auto_ptr<User> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, User o);
class UserGroup : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class UserHandle : public litesql::RelationHandle<UserGroup> {
    public:
        UserHandle(const UserGroup& owner);
        void link(const User& o0);
        void unlink(const User& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<User> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<UserUserGroupRelationUser2UserGroup::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Name;
    litesql::Field<litesql::Date> name;
    static const litesql::FieldType Nodecount;
    litesql::Field<int> nodecount;
protected:
    void defaults();
public:
    UserGroup(const litesql::Database& db);
    UserGroup(const litesql::Database& db, const litesql::Record& rec);
    UserGroup(const UserGroup& obj);
    const UserGroup& operator=(const UserGroup& obj);
    UserGroup::UserHandle user();
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
    std::auto_ptr<UserGroup> upcast();
    std::auto_ptr<UserGroup> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, UserGroup o);
class HiveDb : public litesql::Database {
public:
    HiveDb(std::string backendType, std::string connInfo);
protected:
    virtual std::vector<litesql::Database::SchemaItem> getSchema() const;
    static void initialize();
};
}
#endif
