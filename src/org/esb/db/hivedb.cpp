#include "hivedb.hpp"
namespace db {
using namespace litesql;
FilterFilterParameterRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : filterParameter(FilterFilterParameterRelation::FilterParameter), filter(FilterFilterParameterRelation::Filter) {
    switch(rec.size()) {
    case 2:
        filterParameter = rec[1];
    case 1:
        filter = rec[0];
    }
}
const std::string FilterFilterParameterRelation::table__("Filter_FilterParameter_");
const litesql::FieldType FilterFilterParameterRelation::Filter("Filter1","INTEGER",table__);
const litesql::FieldType FilterFilterParameterRelation::FilterParameter("FilterParameter2","INTEGER",table__);
void FilterFilterParameterRelation::link(const litesql::Database& db, const db::Filter& o0, const db::FilterParameter& o1) {
    Record values;
    Split fields;
    fields.push_back(Filter.name());
    values.push_back(o0.id);
    fields.push_back(FilterParameter.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void FilterFilterParameterRelation::unlink(const litesql::Database& db, const db::Filter& o0, const db::FilterParameter& o1) {
    db.delete_(table__, (Filter == o0.id && FilterParameter == o1.id));
}
void FilterFilterParameterRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<FilterFilterParameterRelation::Row> FilterFilterParameterRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Filter.fullName());
    sel.result(FilterParameter.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<FilterFilterParameterRelation::Row>(db, sel);
}
template <> litesql::DataSource<db::Filter> FilterFilterParameterRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Filter.fullName());
    sel.where(srcExpr);
    return DataSource<db::Filter>(db, db::Filter::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::FilterParameter> FilterFilterParameterRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(FilterParameter.fullName());
    sel.where(srcExpr);
    return DataSource<db::FilterParameter>(db, db::FilterParameter::Id.in(sel) && expr);
}
MediaFileProjectRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : project(MediaFileProjectRelation::Project), mediaFile(MediaFileProjectRelation::MediaFile) {
    switch(rec.size()) {
    case 2:
        project = rec[1];
    case 1:
        mediaFile = rec[0];
    }
}
const std::string MediaFileProjectRelation::table__("MediaFile_Project_");
const litesql::FieldType MediaFileProjectRelation::MediaFile("MediaFile1","INTEGER",table__);
const litesql::FieldType MediaFileProjectRelation::Project("Project2","INTEGER",table__);
void MediaFileProjectRelation::link(const litesql::Database& db, const db::MediaFile& o0, const db::Project& o1) {
    Record values;
    Split fields;
    fields.push_back(MediaFile.name());
    values.push_back(o0.id);
    fields.push_back(Project.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void MediaFileProjectRelation::unlink(const litesql::Database& db, const db::MediaFile& o0, const db::Project& o1) {
    db.delete_(table__, (MediaFile == o0.id && Project == o1.id));
}
void MediaFileProjectRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<MediaFileProjectRelation::Row> MediaFileProjectRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(MediaFile.fullName());
    sel.result(Project.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<MediaFileProjectRelation::Row>(db, sel);
}
template <> litesql::DataSource<db::MediaFile> MediaFileProjectRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(MediaFile.fullName());
    sel.where(srcExpr);
    return DataSource<db::MediaFile>(db, db::MediaFile::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::Project> MediaFileProjectRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Project.fullName());
    sel.where(srcExpr);
    return DataSource<db::Project>(db, db::Project::Id.in(sel) && expr);
}
ProfileProjectRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : project(ProfileProjectRelation::Project), profile(ProfileProjectRelation::Profile) {
    switch(rec.size()) {
    case 2:
        project = rec[1];
    case 1:
        profile = rec[0];
    }
}
const std::string ProfileProjectRelation::table__("Profile_Project_");
const litesql::FieldType ProfileProjectRelation::Profile("Profile1","INTEGER",table__);
const litesql::FieldType ProfileProjectRelation::Project("Project2","INTEGER",table__);
void ProfileProjectRelation::link(const litesql::Database& db, const db::Profile& o0, const db::Project& o1) {
    Record values;
    Split fields;
    fields.push_back(Profile.name());
    values.push_back(o0.id);
    fields.push_back(Project.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void ProfileProjectRelation::unlink(const litesql::Database& db, const db::Profile& o0, const db::Project& o1) {
    db.delete_(table__, (Profile == o0.id && Project == o1.id));
}
void ProfileProjectRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<ProfileProjectRelation::Row> ProfileProjectRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Profile.fullName());
    sel.result(Project.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<ProfileProjectRelation::Row>(db, sel);
}
template <> litesql::DataSource<db::Profile> ProfileProjectRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Profile.fullName());
    sel.where(srcExpr);
    return DataSource<db::Profile>(db, db::Profile::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::Project> ProfileProjectRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Project.fullName());
    sel.where(srcExpr);
    return DataSource<db::Project>(db, db::Project::Id.in(sel) && expr);
}
MediaFileStreamRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : stream(MediaFileStreamRelation::Stream), mediaFile(MediaFileStreamRelation::MediaFile) {
    switch(rec.size()) {
    case 2:
        stream = rec[1];
    case 1:
        mediaFile = rec[0];
    }
}
const std::string MediaFileStreamRelation::table__("MediaFile_Stream_");
const litesql::FieldType MediaFileStreamRelation::MediaFile("MediaFile1","INTEGER",table__);
const litesql::FieldType MediaFileStreamRelation::Stream("Stream2","INTEGER",table__);
void MediaFileStreamRelation::link(const litesql::Database& db, const db::MediaFile& o0, const db::Stream& o1) {
    Record values;
    Split fields;
    fields.push_back(MediaFile.name());
    values.push_back(o0.id);
    fields.push_back(Stream.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void MediaFileStreamRelation::unlink(const litesql::Database& db, const db::MediaFile& o0, const db::Stream& o1) {
    db.delete_(table__, (MediaFile == o0.id && Stream == o1.id));
}
void MediaFileStreamRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<MediaFileStreamRelation::Row> MediaFileStreamRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(MediaFile.fullName());
    sel.result(Stream.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<MediaFileStreamRelation::Row>(db, sel);
}
template <> litesql::DataSource<db::MediaFile> MediaFileStreamRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(MediaFile.fullName());
    sel.where(srcExpr);
    return DataSource<db::MediaFile>(db, db::MediaFile::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::Stream> MediaFileStreamRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Stream.fullName());
    sel.where(srcExpr);
    return DataSource<db::Stream>(db, db::Stream::Id.in(sel) && expr);
}
JobMediaFileRelationJobInFile::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : mediaFile(JobMediaFileRelationJobInFile::MediaFile), job(JobMediaFileRelationJobInFile::Job) {
    switch(rec.size()) {
    case 2:
        mediaFile = rec[1];
    case 1:
        job = rec[0];
    }
}
const std::string JobMediaFileRelationJobInFile::table__("Job_MediaFile_JobInFile");
const litesql::FieldType JobMediaFileRelationJobInFile::Job("Job1","INTEGER",table__);
const litesql::FieldType JobMediaFileRelationJobInFile::MediaFile("MediaFile2","INTEGER",table__);
void JobMediaFileRelationJobInFile::link(const litesql::Database& db, const db::Job& o0, const db::MediaFile& o1) {
    Record values;
    Split fields;
    fields.push_back(Job.name());
    values.push_back(o0.id);
    fields.push_back(MediaFile.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void JobMediaFileRelationJobInFile::unlink(const litesql::Database& db, const db::Job& o0, const db::MediaFile& o1) {
    db.delete_(table__, (Job == o0.id && MediaFile == o1.id));
}
void JobMediaFileRelationJobInFile::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<JobMediaFileRelationJobInFile::Row> JobMediaFileRelationJobInFile::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Job.fullName());
    sel.result(MediaFile.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<JobMediaFileRelationJobInFile::Row>(db, sel);
}
template <> litesql::DataSource<db::Job> JobMediaFileRelationJobInFile::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Job.fullName());
    sel.where(srcExpr);
    return DataSource<db::Job>(db, db::Job::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::MediaFile> JobMediaFileRelationJobInFile::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(MediaFile.fullName());
    sel.where(srcExpr);
    return DataSource<db::MediaFile>(db, db::MediaFile::Id.in(sel) && expr);
}
JobMediaFileRelationJobOutFile::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : mediaFile(JobMediaFileRelationJobOutFile::MediaFile), job(JobMediaFileRelationJobOutFile::Job) {
    switch(rec.size()) {
    case 2:
        mediaFile = rec[1];
    case 1:
        job = rec[0];
    }
}
const std::string JobMediaFileRelationJobOutFile::table__("Job_MediaFile_JobOutFile");
const litesql::FieldType JobMediaFileRelationJobOutFile::Job("Job1","INTEGER",table__);
const litesql::FieldType JobMediaFileRelationJobOutFile::MediaFile("MediaFile2","INTEGER",table__);
void JobMediaFileRelationJobOutFile::link(const litesql::Database& db, const db::Job& o0, const db::MediaFile& o1) {
    Record values;
    Split fields;
    fields.push_back(Job.name());
    values.push_back(o0.id);
    fields.push_back(MediaFile.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void JobMediaFileRelationJobOutFile::unlink(const litesql::Database& db, const db::Job& o0, const db::MediaFile& o1) {
    db.delete_(table__, (Job == o0.id && MediaFile == o1.id));
}
void JobMediaFileRelationJobOutFile::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<JobMediaFileRelationJobOutFile::Row> JobMediaFileRelationJobOutFile::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Job.fullName());
    sel.result(MediaFile.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<JobMediaFileRelationJobOutFile::Row>(db, sel);
}
template <> litesql::DataSource<db::Job> JobMediaFileRelationJobOutFile::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Job.fullName());
    sel.where(srcExpr);
    return DataSource<db::Job>(db, db::Job::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::MediaFile> JobMediaFileRelationJobOutFile::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(MediaFile.fullName());
    sel.where(srcExpr);
    return DataSource<db::MediaFile>(db, db::MediaFile::Id.in(sel) && expr);
}
JobJobDetailRelationJobJobDetail::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : jobDetail(JobJobDetailRelationJobJobDetail::JobDetail), job(JobJobDetailRelationJobJobDetail::Job) {
    switch(rec.size()) {
    case 2:
        jobDetail = rec[1];
    case 1:
        job = rec[0];
    }
}
const std::string JobJobDetailRelationJobJobDetail::table__("Job_JobDetail_JobJobDetail");
const litesql::FieldType JobJobDetailRelationJobJobDetail::Job("Job1","INTEGER",table__);
const litesql::FieldType JobJobDetailRelationJobJobDetail::JobDetail("JobDetail2","INTEGER",table__);
void JobJobDetailRelationJobJobDetail::link(const litesql::Database& db, const db::Job& o0, const db::JobDetail& o1) {
    Record values;
    Split fields;
    fields.push_back(Job.name());
    values.push_back(o0.id);
    fields.push_back(JobDetail.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void JobJobDetailRelationJobJobDetail::unlink(const litesql::Database& db, const db::Job& o0, const db::JobDetail& o1) {
    db.delete_(table__, (Job == o0.id && JobDetail == o1.id));
}
void JobJobDetailRelationJobJobDetail::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<JobJobDetailRelationJobJobDetail::Row> JobJobDetailRelationJobJobDetail::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Job.fullName());
    sel.result(JobDetail.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<JobJobDetailRelationJobJobDetail::Row>(db, sel);
}
template <> litesql::DataSource<db::Job> JobJobDetailRelationJobJobDetail::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Job.fullName());
    sel.where(srcExpr);
    return DataSource<db::Job>(db, db::Job::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::JobDetail> JobJobDetailRelationJobJobDetail::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(JobDetail.fullName());
    sel.where(srcExpr);
    return DataSource<db::JobDetail>(db, db::JobDetail::Id.in(sel) && expr);
}
JobDetailStreamRelationJobOutStream::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : stream(JobDetailStreamRelationJobOutStream::Stream), jobDetail(JobDetailStreamRelationJobOutStream::JobDetail) {
    switch(rec.size()) {
    case 2:
        stream = rec[1];
    case 1:
        jobDetail = rec[0];
    }
}
const std::string JobDetailStreamRelationJobOutStream::table__("JobDetail_Stream_JobOutStream");
const litesql::FieldType JobDetailStreamRelationJobOutStream::JobDetail("JobDetail1","INTEGER",table__);
const litesql::FieldType JobDetailStreamRelationJobOutStream::Stream("Stream2","INTEGER",table__);
void JobDetailStreamRelationJobOutStream::link(const litesql::Database& db, const db::JobDetail& o0, const db::Stream& o1) {
    Record values;
    Split fields;
    fields.push_back(JobDetail.name());
    values.push_back(o0.id);
    fields.push_back(Stream.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void JobDetailStreamRelationJobOutStream::unlink(const litesql::Database& db, const db::JobDetail& o0, const db::Stream& o1) {
    db.delete_(table__, (JobDetail == o0.id && Stream == o1.id));
}
void JobDetailStreamRelationJobOutStream::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<JobDetailStreamRelationJobOutStream::Row> JobDetailStreamRelationJobOutStream::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(JobDetail.fullName());
    sel.result(Stream.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<JobDetailStreamRelationJobOutStream::Row>(db, sel);
}
template <> litesql::DataSource<db::JobDetail> JobDetailStreamRelationJobOutStream::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(JobDetail.fullName());
    sel.where(srcExpr);
    return DataSource<db::JobDetail>(db, db::JobDetail::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::Stream> JobDetailStreamRelationJobOutStream::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Stream.fullName());
    sel.where(srcExpr);
    return DataSource<db::Stream>(db, db::Stream::Id.in(sel) && expr);
}
JobDetailStreamRelationJobInStream::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : stream(JobDetailStreamRelationJobInStream::Stream), jobDetail(JobDetailStreamRelationJobInStream::JobDetail) {
    switch(rec.size()) {
    case 2:
        stream = rec[1];
    case 1:
        jobDetail = rec[0];
    }
}
const std::string JobDetailStreamRelationJobInStream::table__("JobDetail_Stream_JobInStream");
const litesql::FieldType JobDetailStreamRelationJobInStream::JobDetail("JobDetail1","INTEGER",table__);
const litesql::FieldType JobDetailStreamRelationJobInStream::Stream("Stream2","INTEGER",table__);
void JobDetailStreamRelationJobInStream::link(const litesql::Database& db, const db::JobDetail& o0, const db::Stream& o1) {
    Record values;
    Split fields;
    fields.push_back(JobDetail.name());
    values.push_back(o0.id);
    fields.push_back(Stream.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void JobDetailStreamRelationJobInStream::unlink(const litesql::Database& db, const db::JobDetail& o0, const db::Stream& o1) {
    db.delete_(table__, (JobDetail == o0.id && Stream == o1.id));
}
void JobDetailStreamRelationJobInStream::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<JobDetailStreamRelationJobInStream::Row> JobDetailStreamRelationJobInStream::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(JobDetail.fullName());
    sel.result(Stream.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<JobDetailStreamRelationJobInStream::Row>(db, sel);
}
template <> litesql::DataSource<db::JobDetail> JobDetailStreamRelationJobInStream::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(JobDetail.fullName());
    sel.where(srcExpr);
    return DataSource<db::JobDetail>(db, db::JobDetail::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::Stream> JobDetailStreamRelationJobInStream::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Stream.fullName());
    sel.where(srcExpr);
    return DataSource<db::Stream>(db, db::Stream::Id.in(sel) && expr);
}
ProfileWatchfolderRelationWatchfolderProfile::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : watchfolder(ProfileWatchfolderRelationWatchfolderProfile::Watchfolder), profile(ProfileWatchfolderRelationWatchfolderProfile::Profile) {
    switch(rec.size()) {
    case 2:
        watchfolder = rec[1];
    case 1:
        profile = rec[0];
    }
}
const std::string ProfileWatchfolderRelationWatchfolderProfile::table__("_72915fab98e40e57ddd1495ecd15b95b");
const litesql::FieldType ProfileWatchfolderRelationWatchfolderProfile::Profile("Profile1","INTEGER",table__);
const litesql::FieldType ProfileWatchfolderRelationWatchfolderProfile::Watchfolder("Watchfolder2","INTEGER",table__);
void ProfileWatchfolderRelationWatchfolderProfile::link(const litesql::Database& db, const db::Profile& o0, const db::Watchfolder& o1) {
    Record values;
    Split fields;
    fields.push_back(Profile.name());
    values.push_back(o0.id);
    fields.push_back(Watchfolder.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void ProfileWatchfolderRelationWatchfolderProfile::unlink(const litesql::Database& db, const db::Profile& o0, const db::Watchfolder& o1) {
    db.delete_(table__, (Profile == o0.id && Watchfolder == o1.id));
}
void ProfileWatchfolderRelationWatchfolderProfile::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<ProfileWatchfolderRelationWatchfolderProfile::Row> ProfileWatchfolderRelationWatchfolderProfile::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Profile.fullName());
    sel.result(Watchfolder.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<ProfileWatchfolderRelationWatchfolderProfile::Row>(db, sel);
}
template <> litesql::DataSource<db::Profile> ProfileWatchfolderRelationWatchfolderProfile::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Profile.fullName());
    sel.where(srcExpr);
    return DataSource<db::Profile>(db, db::Profile::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::Watchfolder> ProfileWatchfolderRelationWatchfolderProfile::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Watchfolder.fullName());
    sel.where(srcExpr);
    return DataSource<db::Watchfolder>(db, db::Watchfolder::Id.in(sel) && expr);
}
const litesql::FieldType Project::Own::Id("id_","INTEGER","Project_");
Project::MediafilesHandle::MediafilesHandle(const Project& owner)
         : litesql::RelationHandle<Project>(owner) {
}
void Project::MediafilesHandle::link(const MediaFile& o0) {
    MediaFileProjectRelation::link(owner->getDatabase(), o0, *owner);
}
void Project::MediafilesHandle::unlink(const MediaFile& o0) {
    MediaFileProjectRelation::unlink(owner->getDatabase(), o0, *owner);
}
void Project::MediafilesHandle::del(const litesql::Expr& expr) {
    MediaFileProjectRelation::del(owner->getDatabase(), expr && MediaFileProjectRelation::Project == owner->id);
}
litesql::DataSource<MediaFile> Project::MediafilesHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return MediaFileProjectRelation::get<MediaFile>(owner->getDatabase(), expr, (MediaFileProjectRelation::Project == owner->id) && srcExpr);
}
litesql::DataSource<MediaFileProjectRelation::Row> Project::MediafilesHandle::getRows(const litesql::Expr& expr) {
    return MediaFileProjectRelation::getRows(owner->getDatabase(), expr && (MediaFileProjectRelation::Project == owner->id));
}
Project::ProfilesHandle::ProfilesHandle(const Project& owner)
         : litesql::RelationHandle<Project>(owner) {
}
void Project::ProfilesHandle::link(const Profile& o0) {
    ProfileProjectRelation::link(owner->getDatabase(), o0, *owner);
}
void Project::ProfilesHandle::unlink(const Profile& o0) {
    ProfileProjectRelation::unlink(owner->getDatabase(), o0, *owner);
}
void Project::ProfilesHandle::del(const litesql::Expr& expr) {
    ProfileProjectRelation::del(owner->getDatabase(), expr && ProfileProjectRelation::Project == owner->id);
}
litesql::DataSource<Profile> Project::ProfilesHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return ProfileProjectRelation::get<Profile>(owner->getDatabase(), expr, (ProfileProjectRelation::Project == owner->id) && srcExpr);
}
litesql::DataSource<ProfileProjectRelation::Row> Project::ProfilesHandle::getRows(const litesql::Expr& expr) {
    return ProfileProjectRelation::getRows(owner->getDatabase(), expr && (ProfileProjectRelation::Project == owner->id));
}
const std::string Project::type__("Project");
const std::string Project::table__("Project_");
const std::string Project::sequence__("Project_seq");
const litesql::FieldType Project::Id("id_","INTEGER",table__);
const litesql::FieldType Project::Type("type_","TEXT",table__);
const litesql::FieldType Project::Name("name_","TEXT",table__);
const litesql::FieldType Project::Created("created_","INTEGER",table__);
void Project::defaults() {
    id = 0;
    created = 0;
}
Project::Project(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name), created(Created) {
    defaults();
}
Project::Project(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name), created(Created) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: created = convert<const std::string&, litesql::Date>(rec[3]);
        created.setModified(false);
    case 3: name = convert<const std::string&, std::string>(rec[2]);
        name.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Project::Project(const Project& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name), created(obj.created) {
}
const Project& Project::operator=(const Project& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
        created = obj.created;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Project::MediafilesHandle Project::mediafiles() {
    return Project::MediafilesHandle(*this);
}
Project::ProfilesHandle Project::profiles() {
    return Project::ProfilesHandle(*this);
}
std::string Project::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(name.name());
    values.push_back(name);
    name.setModified(false);
    fields.push_back(created.name());
    values.push_back(created);
    created.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Project::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Project::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, name);
    updateField(updates, table__, created);
}
void Project::addIDUpdates(Updates& updates) {
}
void Project::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
    ftypes.push_back(Created);
}
void Project::delRecord() {
    deleteFromTable(table__, id);
}
void Project::delRelations() {
    MediaFileProjectRelation::del(*db, (MediaFileProjectRelation::Project == id));
    ProfileProjectRelation::del(*db, (ProfileProjectRelation::Project == id));
}
void Project::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void Project::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<Project> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Project::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<Project> Project::upcast() {
    return auto_ptr<Project>(new Project(*this));
}
std::auto_ptr<Project> Project::upcastCopy() {
    Project* np = new Project(*this);
    np->id = id;
    np->type = type;
    np->name = name;
    np->created = created;
    np->inDatabase = inDatabase;
    return auto_ptr<Project>(np);
}
std::ostream & operator<<(std::ostream& os, Project o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.created.name() << " = " << o.created << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Filter::Own::Id("id_","INTEGER","Filter_");
Filter::ParameterHandle::ParameterHandle(const Filter& owner)
         : litesql::RelationHandle<Filter>(owner) {
}
void Filter::ParameterHandle::link(const FilterParameter& o0) {
    FilterFilterParameterRelation::link(owner->getDatabase(), *owner, o0);
}
void Filter::ParameterHandle::unlink(const FilterParameter& o0) {
    FilterFilterParameterRelation::unlink(owner->getDatabase(), *owner, o0);
}
void Filter::ParameterHandle::del(const litesql::Expr& expr) {
    FilterFilterParameterRelation::del(owner->getDatabase(), expr && FilterFilterParameterRelation::Filter == owner->id);
}
litesql::DataSource<FilterParameter> Filter::ParameterHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FilterFilterParameterRelation::get<FilterParameter>(owner->getDatabase(), expr, (FilterFilterParameterRelation::Filter == owner->id) && srcExpr);
}
litesql::DataSource<FilterFilterParameterRelation::Row> Filter::ParameterHandle::getRows(const litesql::Expr& expr) {
    return FilterFilterParameterRelation::getRows(owner->getDatabase(), expr && (FilterFilterParameterRelation::Filter == owner->id));
}
const std::string Filter::type__("Filter");
const std::string Filter::table__("Filter_");
const std::string Filter::sequence__("Filter_seq");
const litesql::FieldType Filter::Id("id_","INTEGER",table__);
const litesql::FieldType Filter::Type("type_","TEXT",table__);
const litesql::FieldType Filter::Filtername("filtername_","TEXT",table__);
const litesql::FieldType Filter::Filterid("filterid_","TEXT",table__);
void Filter::defaults() {
    id = 0;
}
Filter::Filter(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), filtername(Filtername), filterid(Filterid) {
    defaults();
}
Filter::Filter(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), filtername(Filtername), filterid(Filterid) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: filterid = convert<const std::string&, std::string>(rec[3]);
        filterid.setModified(false);
    case 3: filtername = convert<const std::string&, std::string>(rec[2]);
        filtername.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Filter::Filter(const Filter& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), filtername(obj.filtername), filterid(obj.filterid) {
}
const Filter& Filter::operator=(const Filter& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        filtername = obj.filtername;
        filterid = obj.filterid;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Filter::ParameterHandle Filter::parameter() {
    return Filter::ParameterHandle(*this);
}
std::string Filter::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(filtername.name());
    values.push_back(filtername);
    filtername.setModified(false);
    fields.push_back(filterid.name());
    values.push_back(filterid);
    filterid.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Filter::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Filter::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, filtername);
    updateField(updates, table__, filterid);
}
void Filter::addIDUpdates(Updates& updates) {
}
void Filter::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Filtername);
    ftypes.push_back(Filterid);
}
void Filter::delRecord() {
    deleteFromTable(table__, id);
}
void Filter::delRelations() {
    FilterFilterParameterRelation::del(*db, (FilterFilterParameterRelation::Filter == id));
}
void Filter::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void Filter::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<Filter> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Filter::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<Filter> Filter::upcast() {
    return auto_ptr<Filter>(new Filter(*this));
}
std::auto_ptr<Filter> Filter::upcastCopy() {
    Filter* np = new Filter(*this);
    np->id = id;
    np->type = type;
    np->filtername = filtername;
    np->filterid = filterid;
    np->inDatabase = inDatabase;
    return auto_ptr<Filter>(np);
}
std::ostream & operator<<(std::ostream& os, Filter o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.filtername.name() << " = " << o.filtername << std::endl;
    os << o.filterid.name() << " = " << o.filterid << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType FilterParameter::Own::Id("id_","INTEGER","FilterParameter_");
FilterParameter::FilterHandle::FilterHandle(const FilterParameter& owner)
         : litesql::RelationHandle<FilterParameter>(owner) {
}
void FilterParameter::FilterHandle::link(const Filter& o0) {
    FilterFilterParameterRelation::link(owner->getDatabase(), o0, *owner);
}
void FilterParameter::FilterHandle::unlink(const Filter& o0) {
    FilterFilterParameterRelation::unlink(owner->getDatabase(), o0, *owner);
}
void FilterParameter::FilterHandle::del(const litesql::Expr& expr) {
    FilterFilterParameterRelation::del(owner->getDatabase(), expr && FilterFilterParameterRelation::FilterParameter == owner->id);
}
litesql::DataSource<Filter> FilterParameter::FilterHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FilterFilterParameterRelation::get<Filter>(owner->getDatabase(), expr, (FilterFilterParameterRelation::FilterParameter == owner->id) && srcExpr);
}
litesql::DataSource<FilterFilterParameterRelation::Row> FilterParameter::FilterHandle::getRows(const litesql::Expr& expr) {
    return FilterFilterParameterRelation::getRows(owner->getDatabase(), expr && (FilterFilterParameterRelation::FilterParameter == owner->id));
}
const std::string FilterParameter::type__("FilterParameter");
const std::string FilterParameter::table__("FilterParameter_");
const std::string FilterParameter::sequence__("FilterParameter_seq");
const litesql::FieldType FilterParameter::Id("id_","INTEGER",table__);
const litesql::FieldType FilterParameter::Type("type_","TEXT",table__);
const litesql::FieldType FilterParameter::Fkey("fkey_","TEXT",table__);
const litesql::FieldType FilterParameter::Fval("fval_","TEXT",table__);
void FilterParameter::defaults() {
    id = 0;
}
FilterParameter::FilterParameter(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), fkey(Fkey), fval(Fval) {
    defaults();
}
FilterParameter::FilterParameter(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), fkey(Fkey), fval(Fval) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: fval = convert<const std::string&, std::string>(rec[3]);
        fval.setModified(false);
    case 3: fkey = convert<const std::string&, std::string>(rec[2]);
        fkey.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
FilterParameter::FilterParameter(const FilterParameter& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), fkey(obj.fkey), fval(obj.fval) {
}
const FilterParameter& FilterParameter::operator=(const FilterParameter& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        fkey = obj.fkey;
        fval = obj.fval;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
FilterParameter::FilterHandle FilterParameter::filter() {
    return FilterParameter::FilterHandle(*this);
}
std::string FilterParameter::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(fkey.name());
    values.push_back(fkey);
    fkey.setModified(false);
    fields.push_back(fval.name());
    values.push_back(fval);
    fval.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void FilterParameter::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void FilterParameter::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, fkey);
    updateField(updates, table__, fval);
}
void FilterParameter::addIDUpdates(Updates& updates) {
}
void FilterParameter::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Fkey);
    ftypes.push_back(Fval);
}
void FilterParameter::delRecord() {
    deleteFromTable(table__, id);
}
void FilterParameter::delRelations() {
    FilterFilterParameterRelation::del(*db, (FilterFilterParameterRelation::FilterParameter == id));
}
void FilterParameter::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void FilterParameter::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<FilterParameter> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool FilterParameter::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<FilterParameter> FilterParameter::upcast() {
    return auto_ptr<FilterParameter>(new FilterParameter(*this));
}
std::auto_ptr<FilterParameter> FilterParameter::upcastCopy() {
    FilterParameter* np = new FilterParameter(*this);
    np->id = id;
    np->type = type;
    np->fkey = fkey;
    np->fval = fval;
    np->inDatabase = inDatabase;
    return auto_ptr<FilterParameter>(np);
}
std::ostream & operator<<(std::ostream& os, FilterParameter o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.fkey.name() << " = " << o.fkey << std::endl;
    os << o.fval.name() << " = " << o.fval << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType MediaFile::Own::Id("id_","INTEGER","MediaFile_");
MediaFile::ProjectHandle::ProjectHandle(const MediaFile& owner)
         : litesql::RelationHandle<MediaFile>(owner) {
}
void MediaFile::ProjectHandle::link(const Project& o0) {
    MediaFileProjectRelation::link(owner->getDatabase(), *owner, o0);
}
void MediaFile::ProjectHandle::unlink(const Project& o0) {
    MediaFileProjectRelation::unlink(owner->getDatabase(), *owner, o0);
}
void MediaFile::ProjectHandle::del(const litesql::Expr& expr) {
    MediaFileProjectRelation::del(owner->getDatabase(), expr && MediaFileProjectRelation::MediaFile == owner->id);
}
litesql::DataSource<Project> MediaFile::ProjectHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return MediaFileProjectRelation::get<Project>(owner->getDatabase(), expr, (MediaFileProjectRelation::MediaFile == owner->id) && srcExpr);
}
litesql::DataSource<MediaFileProjectRelation::Row> MediaFile::ProjectHandle::getRows(const litesql::Expr& expr) {
    return MediaFileProjectRelation::getRows(owner->getDatabase(), expr && (MediaFileProjectRelation::MediaFile == owner->id));
}
MediaFile::StreamsHandle::StreamsHandle(const MediaFile& owner)
         : litesql::RelationHandle<MediaFile>(owner) {
}
void MediaFile::StreamsHandle::link(const Stream& o0) {
    MediaFileStreamRelation::link(owner->getDatabase(), *owner, o0);
}
void MediaFile::StreamsHandle::unlink(const Stream& o0) {
    MediaFileStreamRelation::unlink(owner->getDatabase(), *owner, o0);
}
void MediaFile::StreamsHandle::del(const litesql::Expr& expr) {
    MediaFileStreamRelation::del(owner->getDatabase(), expr && MediaFileStreamRelation::MediaFile == owner->id);
}
litesql::DataSource<Stream> MediaFile::StreamsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return MediaFileStreamRelation::get<Stream>(owner->getDatabase(), expr, (MediaFileStreamRelation::MediaFile == owner->id) && srcExpr);
}
litesql::DataSource<MediaFileStreamRelation::Row> MediaFile::StreamsHandle::getRows(const litesql::Expr& expr) {
    return MediaFileStreamRelation::getRows(owner->getDatabase(), expr && (MediaFileStreamRelation::MediaFile == owner->id));
}
MediaFile::JobsinHandle::JobsinHandle(const MediaFile& owner)
         : litesql::RelationHandle<MediaFile>(owner) {
}
void MediaFile::JobsinHandle::link(const Job& o0) {
    JobMediaFileRelationJobInFile::link(owner->getDatabase(), o0, *owner);
}
void MediaFile::JobsinHandle::unlink(const Job& o0) {
    JobMediaFileRelationJobInFile::unlink(owner->getDatabase(), o0, *owner);
}
void MediaFile::JobsinHandle::del(const litesql::Expr& expr) {
    JobMediaFileRelationJobInFile::del(owner->getDatabase(), expr && JobMediaFileRelationJobInFile::MediaFile == owner->id);
}
litesql::DataSource<Job> MediaFile::JobsinHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobMediaFileRelationJobInFile::get<Job>(owner->getDatabase(), expr, (JobMediaFileRelationJobInFile::MediaFile == owner->id) && srcExpr);
}
litesql::DataSource<JobMediaFileRelationJobInFile::Row> MediaFile::JobsinHandle::getRows(const litesql::Expr& expr) {
    return JobMediaFileRelationJobInFile::getRows(owner->getDatabase(), expr && (JobMediaFileRelationJobInFile::MediaFile == owner->id));
}
MediaFile::JobsoutHandle::JobsoutHandle(const MediaFile& owner)
         : litesql::RelationHandle<MediaFile>(owner) {
}
void MediaFile::JobsoutHandle::link(const Job& o0) {
    JobMediaFileRelationJobOutFile::link(owner->getDatabase(), o0, *owner);
}
void MediaFile::JobsoutHandle::unlink(const Job& o0) {
    JobMediaFileRelationJobOutFile::unlink(owner->getDatabase(), o0, *owner);
}
void MediaFile::JobsoutHandle::del(const litesql::Expr& expr) {
    JobMediaFileRelationJobOutFile::del(owner->getDatabase(), expr && JobMediaFileRelationJobOutFile::MediaFile == owner->id);
}
litesql::DataSource<Job> MediaFile::JobsoutHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobMediaFileRelationJobOutFile::get<Job>(owner->getDatabase(), expr, (JobMediaFileRelationJobOutFile::MediaFile == owner->id) && srcExpr);
}
litesql::DataSource<JobMediaFileRelationJobOutFile::Row> MediaFile::JobsoutHandle::getRows(const litesql::Expr& expr) {
    return JobMediaFileRelationJobOutFile::getRows(owner->getDatabase(), expr && (JobMediaFileRelationJobOutFile::MediaFile == owner->id));
}
const std::string MediaFile::type__("MediaFile");
const std::string MediaFile::table__("MediaFile_");
const std::string MediaFile::sequence__("MediaFile_seq");
const litesql::FieldType MediaFile::Id("id_","INTEGER",table__);
const litesql::FieldType MediaFile::Type("type_","TEXT",table__);
const litesql::FieldType MediaFile::Filename("filename_","TEXT",table__);
const litesql::FieldType MediaFile::Path("path_","TEXT",table__);
const litesql::FieldType MediaFile::Filesize("filesize_","DOUBLE",table__);
const litesql::FieldType MediaFile::Streamcount("streamcount_","INTEGER",table__);
const litesql::FieldType MediaFile::Containertype("containertype_","TEXT",table__);
const litesql::FieldType MediaFile::Duration("duration_","DOUBLE",table__);
const litesql::FieldType MediaFile::Bitrate("bitrate_","INTEGER",table__);
const litesql::FieldType MediaFile::Created("created_","INTEGER",table__);
const litesql::FieldType MediaFile::Filetype("filetype_","INTEGER",table__);
const litesql::FieldType MediaFile::Parent("parent_","INTEGER",table__);
const litesql::FieldType MediaFile::Metatitle("metatitle_","TEXT",table__);
const litesql::FieldType MediaFile::Metaauthor("metaauthor_","TEXT",table__);
const litesql::FieldType MediaFile::Metacopyright("metacopyright_","TEXT",table__);
const litesql::FieldType MediaFile::Metacomment("metacomment_","TEXT",table__);
const litesql::FieldType MediaFile::Metaalbum("metaalbum_","TEXT",table__);
const litesql::FieldType MediaFile::Metayear("metayear_","INTEGER",table__);
const litesql::FieldType MediaFile::Metatrack("metatrack_","INTEGER",table__);
const litesql::FieldType MediaFile::Metagenre("metagenre_","INTEGER",table__);
void MediaFile::defaults() {
    id = 0;
    filesize = 0.0;
    streamcount = 0;
    duration = 0.0;
    bitrate = 0;
    created = 0;
    filetype = 0;
    parent = 0;
    metayear = 0;
    metatrack = 0;
    metagenre = 0;
}
MediaFile::MediaFile(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), filename(Filename), path(Path), filesize(Filesize), streamcount(Streamcount), containertype(Containertype), duration(Duration), bitrate(Bitrate), created(Created), filetype(Filetype), parent(Parent), metatitle(Metatitle), metaauthor(Metaauthor), metacopyright(Metacopyright), metacomment(Metacomment), metaalbum(Metaalbum), metayear(Metayear), metatrack(Metatrack), metagenre(Metagenre) {
    defaults();
}
MediaFile::MediaFile(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), filename(Filename), path(Path), filesize(Filesize), streamcount(Streamcount), containertype(Containertype), duration(Duration), bitrate(Bitrate), created(Created), filetype(Filetype), parent(Parent), metatitle(Metatitle), metaauthor(Metaauthor), metacopyright(Metacopyright), metacomment(Metacomment), metaalbum(Metaalbum), metayear(Metayear), metatrack(Metatrack), metagenre(Metagenre) {
    defaults();
    size_t size = (rec.size() > 20) ? 20 : rec.size();
    switch(size) {
    case 20: metagenre = convert<const std::string&, int>(rec[19]);
        metagenre.setModified(false);
    case 19: metatrack = convert<const std::string&, int>(rec[18]);
        metatrack.setModified(false);
    case 18: metayear = convert<const std::string&, int>(rec[17]);
        metayear.setModified(false);
    case 17: metaalbum = convert<const std::string&, std::string>(rec[16]);
        metaalbum.setModified(false);
    case 16: metacomment = convert<const std::string&, std::string>(rec[15]);
        metacomment.setModified(false);
    case 15: metacopyright = convert<const std::string&, std::string>(rec[14]);
        metacopyright.setModified(false);
    case 14: metaauthor = convert<const std::string&, std::string>(rec[13]);
        metaauthor.setModified(false);
    case 13: metatitle = convert<const std::string&, std::string>(rec[12]);
        metatitle.setModified(false);
    case 12: parent = convert<const std::string&, int>(rec[11]);
        parent.setModified(false);
    case 11: filetype = convert<const std::string&, int>(rec[10]);
        filetype.setModified(false);
    case 10: created = convert<const std::string&, litesql::DateTime>(rec[9]);
        created.setModified(false);
    case 9: bitrate = convert<const std::string&, int>(rec[8]);
        bitrate.setModified(false);
    case 8: duration = convert<const std::string&, double>(rec[7]);
        duration.setModified(false);
    case 7: containertype = convert<const std::string&, std::string>(rec[6]);
        containertype.setModified(false);
    case 6: streamcount = convert<const std::string&, int>(rec[5]);
        streamcount.setModified(false);
    case 5: filesize = convert<const std::string&, double>(rec[4]);
        filesize.setModified(false);
    case 4: path = convert<const std::string&, std::string>(rec[3]);
        path.setModified(false);
    case 3: filename = convert<const std::string&, std::string>(rec[2]);
        filename.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
MediaFile::MediaFile(const MediaFile& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), filename(obj.filename), path(obj.path), filesize(obj.filesize), streamcount(obj.streamcount), containertype(obj.containertype), duration(obj.duration), bitrate(obj.bitrate), created(obj.created), filetype(obj.filetype), parent(obj.parent), metatitle(obj.metatitle), metaauthor(obj.metaauthor), metacopyright(obj.metacopyright), metacomment(obj.metacomment), metaalbum(obj.metaalbum), metayear(obj.metayear), metatrack(obj.metatrack), metagenre(obj.metagenre) {
}
const MediaFile& MediaFile::operator=(const MediaFile& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        filename = obj.filename;
        path = obj.path;
        filesize = obj.filesize;
        streamcount = obj.streamcount;
        containertype = obj.containertype;
        duration = obj.duration;
        bitrate = obj.bitrate;
        created = obj.created;
        filetype = obj.filetype;
        parent = obj.parent;
        metatitle = obj.metatitle;
        metaauthor = obj.metaauthor;
        metacopyright = obj.metacopyright;
        metacomment = obj.metacomment;
        metaalbum = obj.metaalbum;
        metayear = obj.metayear;
        metatrack = obj.metatrack;
        metagenre = obj.metagenre;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
MediaFile::ProjectHandle MediaFile::project() {
    return MediaFile::ProjectHandle(*this);
}
MediaFile::StreamsHandle MediaFile::streams() {
    return MediaFile::StreamsHandle(*this);
}
MediaFile::JobsinHandle MediaFile::jobsin() {
    return MediaFile::JobsinHandle(*this);
}
MediaFile::JobsoutHandle MediaFile::jobsout() {
    return MediaFile::JobsoutHandle(*this);
}
std::string MediaFile::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(filename.name());
    values.push_back(filename);
    filename.setModified(false);
    fields.push_back(path.name());
    values.push_back(path);
    path.setModified(false);
    fields.push_back(filesize.name());
    values.push_back(filesize);
    filesize.setModified(false);
    fields.push_back(streamcount.name());
    values.push_back(streamcount);
    streamcount.setModified(false);
    fields.push_back(containertype.name());
    values.push_back(containertype);
    containertype.setModified(false);
    fields.push_back(duration.name());
    values.push_back(duration);
    duration.setModified(false);
    fields.push_back(bitrate.name());
    values.push_back(bitrate);
    bitrate.setModified(false);
    fields.push_back(created.name());
    values.push_back(created);
    created.setModified(false);
    fields.push_back(filetype.name());
    values.push_back(filetype);
    filetype.setModified(false);
    fields.push_back(parent.name());
    values.push_back(parent);
    parent.setModified(false);
    fields.push_back(metatitle.name());
    values.push_back(metatitle);
    metatitle.setModified(false);
    fields.push_back(metaauthor.name());
    values.push_back(metaauthor);
    metaauthor.setModified(false);
    fields.push_back(metacopyright.name());
    values.push_back(metacopyright);
    metacopyright.setModified(false);
    fields.push_back(metacomment.name());
    values.push_back(metacomment);
    metacomment.setModified(false);
    fields.push_back(metaalbum.name());
    values.push_back(metaalbum);
    metaalbum.setModified(false);
    fields.push_back(metayear.name());
    values.push_back(metayear);
    metayear.setModified(false);
    fields.push_back(metatrack.name());
    values.push_back(metatrack);
    metatrack.setModified(false);
    fields.push_back(metagenre.name());
    values.push_back(metagenre);
    metagenre.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void MediaFile::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void MediaFile::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, filename);
    updateField(updates, table__, path);
    updateField(updates, table__, filesize);
    updateField(updates, table__, streamcount);
    updateField(updates, table__, containertype);
    updateField(updates, table__, duration);
    updateField(updates, table__, bitrate);
    updateField(updates, table__, created);
    updateField(updates, table__, filetype);
    updateField(updates, table__, parent);
    updateField(updates, table__, metatitle);
    updateField(updates, table__, metaauthor);
    updateField(updates, table__, metacopyright);
    updateField(updates, table__, metacomment);
    updateField(updates, table__, metaalbum);
    updateField(updates, table__, metayear);
    updateField(updates, table__, metatrack);
    updateField(updates, table__, metagenre);
}
void MediaFile::addIDUpdates(Updates& updates) {
}
void MediaFile::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Filename);
    ftypes.push_back(Path);
    ftypes.push_back(Filesize);
    ftypes.push_back(Streamcount);
    ftypes.push_back(Containertype);
    ftypes.push_back(Duration);
    ftypes.push_back(Bitrate);
    ftypes.push_back(Created);
    ftypes.push_back(Filetype);
    ftypes.push_back(Parent);
    ftypes.push_back(Metatitle);
    ftypes.push_back(Metaauthor);
    ftypes.push_back(Metacopyright);
    ftypes.push_back(Metacomment);
    ftypes.push_back(Metaalbum);
    ftypes.push_back(Metayear);
    ftypes.push_back(Metatrack);
    ftypes.push_back(Metagenre);
}
void MediaFile::delRecord() {
    deleteFromTable(table__, id);
}
void MediaFile::delRelations() {
    MediaFileProjectRelation::del(*db, (MediaFileProjectRelation::MediaFile == id));
    MediaFileStreamRelation::del(*db, (MediaFileStreamRelation::MediaFile == id));
    JobMediaFileRelationJobInFile::del(*db, (JobMediaFileRelationJobInFile::MediaFile == id));
    JobMediaFileRelationJobOutFile::del(*db, (JobMediaFileRelationJobOutFile::MediaFile == id));
}
void MediaFile::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void MediaFile::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<MediaFile> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool MediaFile::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<MediaFile> MediaFile::upcast() {
    return auto_ptr<MediaFile>(new MediaFile(*this));
}
std::auto_ptr<MediaFile> MediaFile::upcastCopy() {
    MediaFile* np = new MediaFile(*this);
    np->id = id;
    np->type = type;
    np->filename = filename;
    np->path = path;
    np->filesize = filesize;
    np->streamcount = streamcount;
    np->containertype = containertype;
    np->duration = duration;
    np->bitrate = bitrate;
    np->created = created;
    np->filetype = filetype;
    np->parent = parent;
    np->metatitle = metatitle;
    np->metaauthor = metaauthor;
    np->metacopyright = metacopyright;
    np->metacomment = metacomment;
    np->metaalbum = metaalbum;
    np->metayear = metayear;
    np->metatrack = metatrack;
    np->metagenre = metagenre;
    np->inDatabase = inDatabase;
    return auto_ptr<MediaFile>(np);
}
std::ostream & operator<<(std::ostream& os, MediaFile o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.filename.name() << " = " << o.filename << std::endl;
    os << o.path.name() << " = " << o.path << std::endl;
    os << o.filesize.name() << " = " << o.filesize << std::endl;
    os << o.streamcount.name() << " = " << o.streamcount << std::endl;
    os << o.containertype.name() << " = " << o.containertype << std::endl;
    os << o.duration.name() << " = " << o.duration << std::endl;
    os << o.bitrate.name() << " = " << o.bitrate << std::endl;
    os << o.created.name() << " = " << o.created << std::endl;
    os << o.filetype.name() << " = " << o.filetype << std::endl;
    os << o.parent.name() << " = " << o.parent << std::endl;
    os << o.metatitle.name() << " = " << o.metatitle << std::endl;
    os << o.metaauthor.name() << " = " << o.metaauthor << std::endl;
    os << o.metacopyright.name() << " = " << o.metacopyright << std::endl;
    os << o.metacomment.name() << " = " << o.metacomment << std::endl;
    os << o.metaalbum.name() << " = " << o.metaalbum << std::endl;
    os << o.metayear.name() << " = " << o.metayear << std::endl;
    os << o.metatrack.name() << " = " << o.metatrack << std::endl;
    os << o.metagenre.name() << " = " << o.metagenre << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Profile::Own::Id("id_","INTEGER","Profile_");
Profile::ProjectHandle::ProjectHandle(const Profile& owner)
         : litesql::RelationHandle<Profile>(owner) {
}
void Profile::ProjectHandle::link(const Project& o0) {
    ProfileProjectRelation::link(owner->getDatabase(), *owner, o0);
}
void Profile::ProjectHandle::unlink(const Project& o0) {
    ProfileProjectRelation::unlink(owner->getDatabase(), *owner, o0);
}
void Profile::ProjectHandle::del(const litesql::Expr& expr) {
    ProfileProjectRelation::del(owner->getDatabase(), expr && ProfileProjectRelation::Profile == owner->id);
}
litesql::DataSource<Project> Profile::ProjectHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return ProfileProjectRelation::get<Project>(owner->getDatabase(), expr, (ProfileProjectRelation::Profile == owner->id) && srcExpr);
}
litesql::DataSource<ProfileProjectRelation::Row> Profile::ProjectHandle::getRows(const litesql::Expr& expr) {
    return ProfileProjectRelation::getRows(owner->getDatabase(), expr && (ProfileProjectRelation::Profile == owner->id));
}
Profile::WatchfolderHandle::WatchfolderHandle(const Profile& owner)
         : litesql::RelationHandle<Profile>(owner) {
}
void Profile::WatchfolderHandle::link(const Watchfolder& o0) {
    ProfileWatchfolderRelationWatchfolderProfile::link(owner->getDatabase(), *owner, o0);
}
void Profile::WatchfolderHandle::unlink(const Watchfolder& o0) {
    ProfileWatchfolderRelationWatchfolderProfile::unlink(owner->getDatabase(), *owner, o0);
}
void Profile::WatchfolderHandle::del(const litesql::Expr& expr) {
    ProfileWatchfolderRelationWatchfolderProfile::del(owner->getDatabase(), expr && ProfileWatchfolderRelationWatchfolderProfile::Profile == owner->id);
}
litesql::DataSource<Watchfolder> Profile::WatchfolderHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return ProfileWatchfolderRelationWatchfolderProfile::get<Watchfolder>(owner->getDatabase(), expr, (ProfileWatchfolderRelationWatchfolderProfile::Profile == owner->id) && srcExpr);
}
litesql::DataSource<ProfileWatchfolderRelationWatchfolderProfile::Row> Profile::WatchfolderHandle::getRows(const litesql::Expr& expr) {
    return ProfileWatchfolderRelationWatchfolderProfile::getRows(owner->getDatabase(), expr && (ProfileWatchfolderRelationWatchfolderProfile::Profile == owner->id));
}
const std::string Profile::type__("Profile");
const std::string Profile::table__("Profile_");
const std::string Profile::sequence__("Profile_seq");
const litesql::FieldType Profile::Id("id_","INTEGER",table__);
const litesql::FieldType Profile::Type("type_","TEXT",table__);
const litesql::FieldType Profile::Name("name_","TEXT",table__);
const litesql::FieldType Profile::Created("created_","INTEGER",table__);
const litesql::FieldType Profile::Format("format_","TEXT",table__);
const litesql::FieldType Profile::Formatext("formatext_","TEXT",table__);
const litesql::FieldType Profile::Vcodec("vcodec_","INTEGER",table__);
const litesql::FieldType Profile::Vbitrate("vbitrate_","INTEGER",table__);
const litesql::FieldType Profile::Vframerate("vframerate_","TEXT",table__);
const litesql::FieldType Profile::Vwidth("vwidth_","INTEGER",table__);
const litesql::FieldType Profile::Vheight("vheight_","INTEGER",table__);
const litesql::FieldType Profile::Vextra("vextra_","TEXT",table__);
const litesql::FieldType Profile::Achannels("achannels_","INTEGER",table__);
const litesql::FieldType Profile::Acodec("acodec_","INTEGER",table__);
const litesql::FieldType Profile::Abitrate("abitrate_","INTEGER",table__);
const litesql::FieldType Profile::Asamplerate("asamplerate_","INTEGER",table__);
const litesql::FieldType Profile::Aextra("aextra_","TEXT",table__);
const litesql::FieldType Profile::Profiletype("profiletype_","INTEGER",table__);
void Profile::defaults() {
    id = 0;
    created = 0;
    vcodec = 0;
    vbitrate = 0;
    vwidth = 0;
    vheight = 0;
    achannels = 0;
    acodec = 0;
    abitrate = 0;
    asamplerate = 0;
    profiletype = 0;
}
Profile::Profile(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name), created(Created), format(Format), formatext(Formatext), vcodec(Vcodec), vbitrate(Vbitrate), vframerate(Vframerate), vwidth(Vwidth), vheight(Vheight), vextra(Vextra), achannels(Achannels), acodec(Acodec), abitrate(Abitrate), asamplerate(Asamplerate), aextra(Aextra), profiletype(Profiletype) {
    defaults();
}
Profile::Profile(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name), created(Created), format(Format), formatext(Formatext), vcodec(Vcodec), vbitrate(Vbitrate), vframerate(Vframerate), vwidth(Vwidth), vheight(Vheight), vextra(Vextra), achannels(Achannels), acodec(Acodec), abitrate(Abitrate), asamplerate(Asamplerate), aextra(Aextra), profiletype(Profiletype) {
    defaults();
    size_t size = (rec.size() > 18) ? 18 : rec.size();
    switch(size) {
    case 18: profiletype = convert<const std::string&, int>(rec[17]);
        profiletype.setModified(false);
    case 17: aextra = convert<const std::string&, std::string>(rec[16]);
        aextra.setModified(false);
    case 16: asamplerate = convert<const std::string&, int>(rec[15]);
        asamplerate.setModified(false);
    case 15: abitrate = convert<const std::string&, int>(rec[14]);
        abitrate.setModified(false);
    case 14: acodec = convert<const std::string&, int>(rec[13]);
        acodec.setModified(false);
    case 13: achannels = convert<const std::string&, int>(rec[12]);
        achannels.setModified(false);
    case 12: vextra = convert<const std::string&, std::string>(rec[11]);
        vextra.setModified(false);
    case 11: vheight = convert<const std::string&, int>(rec[10]);
        vheight.setModified(false);
    case 10: vwidth = convert<const std::string&, int>(rec[9]);
        vwidth.setModified(false);
    case 9: vframerate = convert<const std::string&, std::string>(rec[8]);
        vframerate.setModified(false);
    case 8: vbitrate = convert<const std::string&, int>(rec[7]);
        vbitrate.setModified(false);
    case 7: vcodec = convert<const std::string&, int>(rec[6]);
        vcodec.setModified(false);
    case 6: formatext = convert<const std::string&, std::string>(rec[5]);
        formatext.setModified(false);
    case 5: format = convert<const std::string&, std::string>(rec[4]);
        format.setModified(false);
    case 4: created = convert<const std::string&, litesql::DateTime>(rec[3]);
        created.setModified(false);
    case 3: name = convert<const std::string&, std::string>(rec[2]);
        name.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Profile::Profile(const Profile& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name), created(obj.created), format(obj.format), formatext(obj.formatext), vcodec(obj.vcodec), vbitrate(obj.vbitrate), vframerate(obj.vframerate), vwidth(obj.vwidth), vheight(obj.vheight), vextra(obj.vextra), achannels(obj.achannels), acodec(obj.acodec), abitrate(obj.abitrate), asamplerate(obj.asamplerate), aextra(obj.aextra), profiletype(obj.profiletype) {
}
const Profile& Profile::operator=(const Profile& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
        created = obj.created;
        format = obj.format;
        formatext = obj.formatext;
        vcodec = obj.vcodec;
        vbitrate = obj.vbitrate;
        vframerate = obj.vframerate;
        vwidth = obj.vwidth;
        vheight = obj.vheight;
        vextra = obj.vextra;
        achannels = obj.achannels;
        acodec = obj.acodec;
        abitrate = obj.abitrate;
        asamplerate = obj.asamplerate;
        aextra = obj.aextra;
        profiletype = obj.profiletype;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Profile::ProjectHandle Profile::project() {
    return Profile::ProjectHandle(*this);
}
Profile::WatchfolderHandle Profile::watchfolder() {
    return Profile::WatchfolderHandle(*this);
}
std::string Profile::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(name.name());
    values.push_back(name);
    name.setModified(false);
    fields.push_back(created.name());
    values.push_back(created);
    created.setModified(false);
    fields.push_back(format.name());
    values.push_back(format);
    format.setModified(false);
    fields.push_back(formatext.name());
    values.push_back(formatext);
    formatext.setModified(false);
    fields.push_back(vcodec.name());
    values.push_back(vcodec);
    vcodec.setModified(false);
    fields.push_back(vbitrate.name());
    values.push_back(vbitrate);
    vbitrate.setModified(false);
    fields.push_back(vframerate.name());
    values.push_back(vframerate);
    vframerate.setModified(false);
    fields.push_back(vwidth.name());
    values.push_back(vwidth);
    vwidth.setModified(false);
    fields.push_back(vheight.name());
    values.push_back(vheight);
    vheight.setModified(false);
    fields.push_back(vextra.name());
    values.push_back(vextra);
    vextra.setModified(false);
    fields.push_back(achannels.name());
    values.push_back(achannels);
    achannels.setModified(false);
    fields.push_back(acodec.name());
    values.push_back(acodec);
    acodec.setModified(false);
    fields.push_back(abitrate.name());
    values.push_back(abitrate);
    abitrate.setModified(false);
    fields.push_back(asamplerate.name());
    values.push_back(asamplerate);
    asamplerate.setModified(false);
    fields.push_back(aextra.name());
    values.push_back(aextra);
    aextra.setModified(false);
    fields.push_back(profiletype.name());
    values.push_back(profiletype);
    profiletype.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Profile::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Profile::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, name);
    updateField(updates, table__, created);
    updateField(updates, table__, format);
    updateField(updates, table__, formatext);
    updateField(updates, table__, vcodec);
    updateField(updates, table__, vbitrate);
    updateField(updates, table__, vframerate);
    updateField(updates, table__, vwidth);
    updateField(updates, table__, vheight);
    updateField(updates, table__, vextra);
    updateField(updates, table__, achannels);
    updateField(updates, table__, acodec);
    updateField(updates, table__, abitrate);
    updateField(updates, table__, asamplerate);
    updateField(updates, table__, aextra);
    updateField(updates, table__, profiletype);
}
void Profile::addIDUpdates(Updates& updates) {
}
void Profile::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
    ftypes.push_back(Created);
    ftypes.push_back(Format);
    ftypes.push_back(Formatext);
    ftypes.push_back(Vcodec);
    ftypes.push_back(Vbitrate);
    ftypes.push_back(Vframerate);
    ftypes.push_back(Vwidth);
    ftypes.push_back(Vheight);
    ftypes.push_back(Vextra);
    ftypes.push_back(Achannels);
    ftypes.push_back(Acodec);
    ftypes.push_back(Abitrate);
    ftypes.push_back(Asamplerate);
    ftypes.push_back(Aextra);
    ftypes.push_back(Profiletype);
}
void Profile::delRecord() {
    deleteFromTable(table__, id);
}
void Profile::delRelations() {
    ProfileProjectRelation::del(*db, (ProfileProjectRelation::Profile == id));
    ProfileWatchfolderRelationWatchfolderProfile::del(*db, (ProfileWatchfolderRelationWatchfolderProfile::Profile == id));
}
void Profile::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void Profile::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<Profile> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Profile::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<Profile> Profile::upcast() {
    return auto_ptr<Profile>(new Profile(*this));
}
std::auto_ptr<Profile> Profile::upcastCopy() {
    Profile* np = new Profile(*this);
    np->id = id;
    np->type = type;
    np->name = name;
    np->created = created;
    np->format = format;
    np->formatext = formatext;
    np->vcodec = vcodec;
    np->vbitrate = vbitrate;
    np->vframerate = vframerate;
    np->vwidth = vwidth;
    np->vheight = vheight;
    np->vextra = vextra;
    np->achannels = achannels;
    np->acodec = acodec;
    np->abitrate = abitrate;
    np->asamplerate = asamplerate;
    np->aextra = aextra;
    np->profiletype = profiletype;
    np->inDatabase = inDatabase;
    return auto_ptr<Profile>(np);
}
std::ostream & operator<<(std::ostream& os, Profile o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.created.name() << " = " << o.created << std::endl;
    os << o.format.name() << " = " << o.format << std::endl;
    os << o.formatext.name() << " = " << o.formatext << std::endl;
    os << o.vcodec.name() << " = " << o.vcodec << std::endl;
    os << o.vbitrate.name() << " = " << o.vbitrate << std::endl;
    os << o.vframerate.name() << " = " << o.vframerate << std::endl;
    os << o.vwidth.name() << " = " << o.vwidth << std::endl;
    os << o.vheight.name() << " = " << o.vheight << std::endl;
    os << o.vextra.name() << " = " << o.vextra << std::endl;
    os << o.achannels.name() << " = " << o.achannels << std::endl;
    os << o.acodec.name() << " = " << o.acodec << std::endl;
    os << o.abitrate.name() << " = " << o.abitrate << std::endl;
    os << o.asamplerate.name() << " = " << o.asamplerate << std::endl;
    os << o.aextra.name() << " = " << o.aextra << std::endl;
    os << o.profiletype.name() << " = " << o.profiletype << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Stream::Own::Id("id_","INTEGER","Stream_");
Stream::MediafileHandle::MediafileHandle(const Stream& owner)
         : litesql::RelationHandle<Stream>(owner) {
}
void Stream::MediafileHandle::link(const MediaFile& o0) {
    MediaFileStreamRelation::link(owner->getDatabase(), o0, *owner);
}
void Stream::MediafileHandle::unlink(const MediaFile& o0) {
    MediaFileStreamRelation::unlink(owner->getDatabase(), o0, *owner);
}
void Stream::MediafileHandle::del(const litesql::Expr& expr) {
    MediaFileStreamRelation::del(owner->getDatabase(), expr && MediaFileStreamRelation::Stream == owner->id);
}
litesql::DataSource<MediaFile> Stream::MediafileHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return MediaFileStreamRelation::get<MediaFile>(owner->getDatabase(), expr, (MediaFileStreamRelation::Stream == owner->id) && srcExpr);
}
litesql::DataSource<MediaFileStreamRelation::Row> Stream::MediafileHandle::getRows(const litesql::Expr& expr) {
    return MediaFileStreamRelation::getRows(owner->getDatabase(), expr && (MediaFileStreamRelation::Stream == owner->id));
}
Stream::JobsoutHandle::JobsoutHandle(const Stream& owner)
         : litesql::RelationHandle<Stream>(owner) {
}
void Stream::JobsoutHandle::link(const JobDetail& o0) {
    JobDetailStreamRelationJobOutStream::link(owner->getDatabase(), o0, *owner);
}
void Stream::JobsoutHandle::unlink(const JobDetail& o0) {
    JobDetailStreamRelationJobOutStream::unlink(owner->getDatabase(), o0, *owner);
}
void Stream::JobsoutHandle::del(const litesql::Expr& expr) {
    JobDetailStreamRelationJobOutStream::del(owner->getDatabase(), expr && JobDetailStreamRelationJobOutStream::Stream == owner->id);
}
litesql::DataSource<JobDetail> Stream::JobsoutHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobDetailStreamRelationJobOutStream::get<JobDetail>(owner->getDatabase(), expr, (JobDetailStreamRelationJobOutStream::Stream == owner->id) && srcExpr);
}
litesql::DataSource<JobDetailStreamRelationJobOutStream::Row> Stream::JobsoutHandle::getRows(const litesql::Expr& expr) {
    return JobDetailStreamRelationJobOutStream::getRows(owner->getDatabase(), expr && (JobDetailStreamRelationJobOutStream::Stream == owner->id));
}
Stream::JobsinHandle::JobsinHandle(const Stream& owner)
         : litesql::RelationHandle<Stream>(owner) {
}
void Stream::JobsinHandle::link(const JobDetail& o0) {
    JobDetailStreamRelationJobInStream::link(owner->getDatabase(), o0, *owner);
}
void Stream::JobsinHandle::unlink(const JobDetail& o0) {
    JobDetailStreamRelationJobInStream::unlink(owner->getDatabase(), o0, *owner);
}
void Stream::JobsinHandle::del(const litesql::Expr& expr) {
    JobDetailStreamRelationJobInStream::del(owner->getDatabase(), expr && JobDetailStreamRelationJobInStream::Stream == owner->id);
}
litesql::DataSource<JobDetail> Stream::JobsinHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobDetailStreamRelationJobInStream::get<JobDetail>(owner->getDatabase(), expr, (JobDetailStreamRelationJobInStream::Stream == owner->id) && srcExpr);
}
litesql::DataSource<JobDetailStreamRelationJobInStream::Row> Stream::JobsinHandle::getRows(const litesql::Expr& expr) {
    return JobDetailStreamRelationJobInStream::getRows(owner->getDatabase(), expr && (JobDetailStreamRelationJobInStream::Stream == owner->id));
}
const std::string Stream::type__("Stream");
const std::string Stream::table__("Stream_");
const std::string Stream::sequence__("Stream_seq");
const litesql::FieldType Stream::Id("id_","INTEGER",table__);
const litesql::FieldType Stream::Type("type_","TEXT",table__);
const litesql::FieldType Stream::Streamindex("streamindex_","INTEGER",table__);
const litesql::FieldType Stream::Streamtype("streamtype_","INTEGER",table__);
const litesql::FieldType Stream::Codecid("codecid_","INTEGER",table__);
const litesql::FieldType Stream::Codecname("codecname_","TEXT",table__);
const litesql::FieldType Stream::Frameratenum("frameratenum_","INTEGER",table__);
const litesql::FieldType Stream::Framerateden("framerateden_","INTEGER",table__);
const litesql::FieldType Stream::Streamtimebasenum("streamtimebasenum_","INTEGER",table__);
const litesql::FieldType Stream::Streamtimebaseden("streamtimebaseden_","INTEGER",table__);
const litesql::FieldType Stream::Codectimebasenum("codectimebasenum_","INTEGER",table__);
const litesql::FieldType Stream::Codectimebaseden("codectimebaseden_","INTEGER",table__);
const litesql::FieldType Stream::Firstpts("firstpts_","DOUBLE",table__);
const litesql::FieldType Stream::Firstdts("firstdts_","DOUBLE",table__);
const litesql::FieldType Stream::Duration("duration_","DOUBLE",table__);
const litesql::FieldType Stream::Nbframes("nbframes_","DOUBLE",table__);
const litesql::FieldType Stream::Ticksperframe("ticksperframe_","INTEGER",table__);
const litesql::FieldType Stream::Framecount("framecount_","INTEGER",table__);
const litesql::FieldType Stream::Width("width_","INTEGER",table__);
const litesql::FieldType Stream::Height("height_","INTEGER",table__);
const litesql::FieldType Stream::Gopsize("gopsize_","INTEGER",table__);
const litesql::FieldType Stream::Pixfmt("pixfmt_","INTEGER",table__);
const litesql::FieldType Stream::Bitrate("bitrate_","INTEGER",table__);
const litesql::FieldType Stream::Samplerate("samplerate_","INTEGER",table__);
const litesql::FieldType Stream::Samplefmt("samplefmt_","INTEGER",table__);
const litesql::FieldType Stream::Channels("channels_","INTEGER",table__);
const litesql::FieldType Stream::Bitspercodedsample("bitspercodedsample_","INTEGER",table__);
const litesql::FieldType Stream::Privdatasize("privdatasize_","INTEGER",table__);
const litesql::FieldType Stream::Privdata("privdata_","TEXT",table__);
const litesql::FieldType Stream::Extradatasize("extradatasize_","INTEGER",table__);
const litesql::FieldType Stream::Extradata("extradata_","TEXT",table__);
const litesql::FieldType Stream::Flags("flags_","INTEGER",table__);
const litesql::FieldType Stream::Extraprofileflags("extraprofileflags_","TEXT",table__);
void Stream::defaults() {
    id = 0;
    streamindex = 0;
    streamtype = 0;
    codecid = 0;
    frameratenum = 0;
    framerateden = 0;
    streamtimebasenum = 0;
    streamtimebaseden = 0;
    codectimebasenum = 0;
    codectimebaseden = 0;
    firstpts = 0.0;
    firstdts = 0.0;
    duration = 0.0;
    nbframes = 0.0;
    ticksperframe = 0;
    framecount = 0;
    width = 0;
    height = 0;
    gopsize = 0;
    pixfmt = 0;
    bitrate = 0;
    samplerate = 0;
    samplefmt = 0;
    channels = 0;
    bitspercodedsample = 0;
    privdatasize = 0;
    extradatasize = 0;
    flags = 0;
}
Stream::Stream(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), streamindex(Streamindex), streamtype(Streamtype), codecid(Codecid), codecname(Codecname), frameratenum(Frameratenum), framerateden(Framerateden), streamtimebasenum(Streamtimebasenum), streamtimebaseden(Streamtimebaseden), codectimebasenum(Codectimebasenum), codectimebaseden(Codectimebaseden), firstpts(Firstpts), firstdts(Firstdts), duration(Duration), nbframes(Nbframes), ticksperframe(Ticksperframe), framecount(Framecount), width(Width), height(Height), gopsize(Gopsize), pixfmt(Pixfmt), bitrate(Bitrate), samplerate(Samplerate), samplefmt(Samplefmt), channels(Channels), bitspercodedsample(Bitspercodedsample), privdatasize(Privdatasize), privdata(Privdata), extradatasize(Extradatasize), extradata(Extradata), flags(Flags), extraprofileflags(Extraprofileflags) {
    defaults();
}
Stream::Stream(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), streamindex(Streamindex), streamtype(Streamtype), codecid(Codecid), codecname(Codecname), frameratenum(Frameratenum), framerateden(Framerateden), streamtimebasenum(Streamtimebasenum), streamtimebaseden(Streamtimebaseden), codectimebasenum(Codectimebasenum), codectimebaseden(Codectimebaseden), firstpts(Firstpts), firstdts(Firstdts), duration(Duration), nbframes(Nbframes), ticksperframe(Ticksperframe), framecount(Framecount), width(Width), height(Height), gopsize(Gopsize), pixfmt(Pixfmt), bitrate(Bitrate), samplerate(Samplerate), samplefmt(Samplefmt), channels(Channels), bitspercodedsample(Bitspercodedsample), privdatasize(Privdatasize), privdata(Privdata), extradatasize(Extradatasize), extradata(Extradata), flags(Flags), extraprofileflags(Extraprofileflags) {
    defaults();
    size_t size = (rec.size() > 33) ? 33 : rec.size();
    switch(size) {
    case 33: extraprofileflags = convert<const std::string&, std::string>(rec[32]);
        extraprofileflags.setModified(false);
    case 32: flags = convert<const std::string&, int>(rec[31]);
        flags.setModified(false);
    case 31: extradata = convert<const std::string&, std::string>(rec[30]);
        extradata.setModified(false);
    case 30: extradatasize = convert<const std::string&, int>(rec[29]);
        extradatasize.setModified(false);
    case 29: privdata = convert<const std::string&, std::string>(rec[28]);
        privdata.setModified(false);
    case 28: privdatasize = convert<const std::string&, int>(rec[27]);
        privdatasize.setModified(false);
    case 27: bitspercodedsample = convert<const std::string&, int>(rec[26]);
        bitspercodedsample.setModified(false);
    case 26: channels = convert<const std::string&, int>(rec[25]);
        channels.setModified(false);
    case 25: samplefmt = convert<const std::string&, int>(rec[24]);
        samplefmt.setModified(false);
    case 24: samplerate = convert<const std::string&, int>(rec[23]);
        samplerate.setModified(false);
    case 23: bitrate = convert<const std::string&, int>(rec[22]);
        bitrate.setModified(false);
    case 22: pixfmt = convert<const std::string&, int>(rec[21]);
        pixfmt.setModified(false);
    case 21: gopsize = convert<const std::string&, int>(rec[20]);
        gopsize.setModified(false);
    case 20: height = convert<const std::string&, int>(rec[19]);
        height.setModified(false);
    case 19: width = convert<const std::string&, int>(rec[18]);
        width.setModified(false);
    case 18: framecount = convert<const std::string&, int>(rec[17]);
        framecount.setModified(false);
    case 17: ticksperframe = convert<const std::string&, int>(rec[16]);
        ticksperframe.setModified(false);
    case 16: nbframes = convert<const std::string&, double>(rec[15]);
        nbframes.setModified(false);
    case 15: duration = convert<const std::string&, double>(rec[14]);
        duration.setModified(false);
    case 14: firstdts = convert<const std::string&, double>(rec[13]);
        firstdts.setModified(false);
    case 13: firstpts = convert<const std::string&, double>(rec[12]);
        firstpts.setModified(false);
    case 12: codectimebaseden = convert<const std::string&, int>(rec[11]);
        codectimebaseden.setModified(false);
    case 11: codectimebasenum = convert<const std::string&, int>(rec[10]);
        codectimebasenum.setModified(false);
    case 10: streamtimebaseden = convert<const std::string&, int>(rec[9]);
        streamtimebaseden.setModified(false);
    case 9: streamtimebasenum = convert<const std::string&, int>(rec[8]);
        streamtimebasenum.setModified(false);
    case 8: framerateden = convert<const std::string&, int>(rec[7]);
        framerateden.setModified(false);
    case 7: frameratenum = convert<const std::string&, int>(rec[6]);
        frameratenum.setModified(false);
    case 6: codecname = convert<const std::string&, std::string>(rec[5]);
        codecname.setModified(false);
    case 5: codecid = convert<const std::string&, int>(rec[4]);
        codecid.setModified(false);
    case 4: streamtype = convert<const std::string&, int>(rec[3]);
        streamtype.setModified(false);
    case 3: streamindex = convert<const std::string&, int>(rec[2]);
        streamindex.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Stream::Stream(const Stream& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), streamindex(obj.streamindex), streamtype(obj.streamtype), codecid(obj.codecid), codecname(obj.codecname), frameratenum(obj.frameratenum), framerateden(obj.framerateden), streamtimebasenum(obj.streamtimebasenum), streamtimebaseden(obj.streamtimebaseden), codectimebasenum(obj.codectimebasenum), codectimebaseden(obj.codectimebaseden), firstpts(obj.firstpts), firstdts(obj.firstdts), duration(obj.duration), nbframes(obj.nbframes), ticksperframe(obj.ticksperframe), framecount(obj.framecount), width(obj.width), height(obj.height), gopsize(obj.gopsize), pixfmt(obj.pixfmt), bitrate(obj.bitrate), samplerate(obj.samplerate), samplefmt(obj.samplefmt), channels(obj.channels), bitspercodedsample(obj.bitspercodedsample), privdatasize(obj.privdatasize), privdata(obj.privdata), extradatasize(obj.extradatasize), extradata(obj.extradata), flags(obj.flags), extraprofileflags(obj.extraprofileflags) {
}
const Stream& Stream::operator=(const Stream& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        streamindex = obj.streamindex;
        streamtype = obj.streamtype;
        codecid = obj.codecid;
        codecname = obj.codecname;
        frameratenum = obj.frameratenum;
        framerateden = obj.framerateden;
        streamtimebasenum = obj.streamtimebasenum;
        streamtimebaseden = obj.streamtimebaseden;
        codectimebasenum = obj.codectimebasenum;
        codectimebaseden = obj.codectimebaseden;
        firstpts = obj.firstpts;
        firstdts = obj.firstdts;
        duration = obj.duration;
        nbframes = obj.nbframes;
        ticksperframe = obj.ticksperframe;
        framecount = obj.framecount;
        width = obj.width;
        height = obj.height;
        gopsize = obj.gopsize;
        pixfmt = obj.pixfmt;
        bitrate = obj.bitrate;
        samplerate = obj.samplerate;
        samplefmt = obj.samplefmt;
        channels = obj.channels;
        bitspercodedsample = obj.bitspercodedsample;
        privdatasize = obj.privdatasize;
        privdata = obj.privdata;
        extradatasize = obj.extradatasize;
        extradata = obj.extradata;
        flags = obj.flags;
        extraprofileflags = obj.extraprofileflags;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Stream::MediafileHandle Stream::mediafile() {
    return Stream::MediafileHandle(*this);
}
Stream::JobsoutHandle Stream::jobsout() {
    return Stream::JobsoutHandle(*this);
}
Stream::JobsinHandle Stream::jobsin() {
    return Stream::JobsinHandle(*this);
}
std::string Stream::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(streamindex.name());
    values.push_back(streamindex);
    streamindex.setModified(false);
    fields.push_back(streamtype.name());
    values.push_back(streamtype);
    streamtype.setModified(false);
    fields.push_back(codecid.name());
    values.push_back(codecid);
    codecid.setModified(false);
    fields.push_back(codecname.name());
    values.push_back(codecname);
    codecname.setModified(false);
    fields.push_back(frameratenum.name());
    values.push_back(frameratenum);
    frameratenum.setModified(false);
    fields.push_back(framerateden.name());
    values.push_back(framerateden);
    framerateden.setModified(false);
    fields.push_back(streamtimebasenum.name());
    values.push_back(streamtimebasenum);
    streamtimebasenum.setModified(false);
    fields.push_back(streamtimebaseden.name());
    values.push_back(streamtimebaseden);
    streamtimebaseden.setModified(false);
    fields.push_back(codectimebasenum.name());
    values.push_back(codectimebasenum);
    codectimebasenum.setModified(false);
    fields.push_back(codectimebaseden.name());
    values.push_back(codectimebaseden);
    codectimebaseden.setModified(false);
    fields.push_back(firstpts.name());
    values.push_back(firstpts);
    firstpts.setModified(false);
    fields.push_back(firstdts.name());
    values.push_back(firstdts);
    firstdts.setModified(false);
    fields.push_back(duration.name());
    values.push_back(duration);
    duration.setModified(false);
    fields.push_back(nbframes.name());
    values.push_back(nbframes);
    nbframes.setModified(false);
    fields.push_back(ticksperframe.name());
    values.push_back(ticksperframe);
    ticksperframe.setModified(false);
    fields.push_back(framecount.name());
    values.push_back(framecount);
    framecount.setModified(false);
    fields.push_back(width.name());
    values.push_back(width);
    width.setModified(false);
    fields.push_back(height.name());
    values.push_back(height);
    height.setModified(false);
    fields.push_back(gopsize.name());
    values.push_back(gopsize);
    gopsize.setModified(false);
    fields.push_back(pixfmt.name());
    values.push_back(pixfmt);
    pixfmt.setModified(false);
    fields.push_back(bitrate.name());
    values.push_back(bitrate);
    bitrate.setModified(false);
    fields.push_back(samplerate.name());
    values.push_back(samplerate);
    samplerate.setModified(false);
    fields.push_back(samplefmt.name());
    values.push_back(samplefmt);
    samplefmt.setModified(false);
    fields.push_back(channels.name());
    values.push_back(channels);
    channels.setModified(false);
    fields.push_back(bitspercodedsample.name());
    values.push_back(bitspercodedsample);
    bitspercodedsample.setModified(false);
    fields.push_back(privdatasize.name());
    values.push_back(privdatasize);
    privdatasize.setModified(false);
    fields.push_back(privdata.name());
    values.push_back(privdata);
    privdata.setModified(false);
    fields.push_back(extradatasize.name());
    values.push_back(extradatasize);
    extradatasize.setModified(false);
    fields.push_back(extradata.name());
    values.push_back(extradata);
    extradata.setModified(false);
    fields.push_back(flags.name());
    values.push_back(flags);
    flags.setModified(false);
    fields.push_back(extraprofileflags.name());
    values.push_back(extraprofileflags);
    extraprofileflags.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Stream::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Stream::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, streamindex);
    updateField(updates, table__, streamtype);
    updateField(updates, table__, codecid);
    updateField(updates, table__, codecname);
    updateField(updates, table__, frameratenum);
    updateField(updates, table__, framerateden);
    updateField(updates, table__, streamtimebasenum);
    updateField(updates, table__, streamtimebaseden);
    updateField(updates, table__, codectimebasenum);
    updateField(updates, table__, codectimebaseden);
    updateField(updates, table__, firstpts);
    updateField(updates, table__, firstdts);
    updateField(updates, table__, duration);
    updateField(updates, table__, nbframes);
    updateField(updates, table__, ticksperframe);
    updateField(updates, table__, framecount);
    updateField(updates, table__, width);
    updateField(updates, table__, height);
    updateField(updates, table__, gopsize);
    updateField(updates, table__, pixfmt);
    updateField(updates, table__, bitrate);
    updateField(updates, table__, samplerate);
    updateField(updates, table__, samplefmt);
    updateField(updates, table__, channels);
    updateField(updates, table__, bitspercodedsample);
    updateField(updates, table__, privdatasize);
    updateField(updates, table__, privdata);
    updateField(updates, table__, extradatasize);
    updateField(updates, table__, extradata);
    updateField(updates, table__, flags);
    updateField(updates, table__, extraprofileflags);
}
void Stream::addIDUpdates(Updates& updates) {
}
void Stream::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Streamindex);
    ftypes.push_back(Streamtype);
    ftypes.push_back(Codecid);
    ftypes.push_back(Codecname);
    ftypes.push_back(Frameratenum);
    ftypes.push_back(Framerateden);
    ftypes.push_back(Streamtimebasenum);
    ftypes.push_back(Streamtimebaseden);
    ftypes.push_back(Codectimebasenum);
    ftypes.push_back(Codectimebaseden);
    ftypes.push_back(Firstpts);
    ftypes.push_back(Firstdts);
    ftypes.push_back(Duration);
    ftypes.push_back(Nbframes);
    ftypes.push_back(Ticksperframe);
    ftypes.push_back(Framecount);
    ftypes.push_back(Width);
    ftypes.push_back(Height);
    ftypes.push_back(Gopsize);
    ftypes.push_back(Pixfmt);
    ftypes.push_back(Bitrate);
    ftypes.push_back(Samplerate);
    ftypes.push_back(Samplefmt);
    ftypes.push_back(Channels);
    ftypes.push_back(Bitspercodedsample);
    ftypes.push_back(Privdatasize);
    ftypes.push_back(Privdata);
    ftypes.push_back(Extradatasize);
    ftypes.push_back(Extradata);
    ftypes.push_back(Flags);
    ftypes.push_back(Extraprofileflags);
}
void Stream::delRecord() {
    deleteFromTable(table__, id);
}
void Stream::delRelations() {
    MediaFileStreamRelation::del(*db, (MediaFileStreamRelation::Stream == id));
    JobDetailStreamRelationJobOutStream::del(*db, (JobDetailStreamRelationJobOutStream::Stream == id));
    JobDetailStreamRelationJobInStream::del(*db, (JobDetailStreamRelationJobInStream::Stream == id));
}
void Stream::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void Stream::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<Stream> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Stream::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<Stream> Stream::upcast() {
    return auto_ptr<Stream>(new Stream(*this));
}
std::auto_ptr<Stream> Stream::upcastCopy() {
    Stream* np = new Stream(*this);
    np->id = id;
    np->type = type;
    np->streamindex = streamindex;
    np->streamtype = streamtype;
    np->codecid = codecid;
    np->codecname = codecname;
    np->frameratenum = frameratenum;
    np->framerateden = framerateden;
    np->streamtimebasenum = streamtimebasenum;
    np->streamtimebaseden = streamtimebaseden;
    np->codectimebasenum = codectimebasenum;
    np->codectimebaseden = codectimebaseden;
    np->firstpts = firstpts;
    np->firstdts = firstdts;
    np->duration = duration;
    np->nbframes = nbframes;
    np->ticksperframe = ticksperframe;
    np->framecount = framecount;
    np->width = width;
    np->height = height;
    np->gopsize = gopsize;
    np->pixfmt = pixfmt;
    np->bitrate = bitrate;
    np->samplerate = samplerate;
    np->samplefmt = samplefmt;
    np->channels = channels;
    np->bitspercodedsample = bitspercodedsample;
    np->privdatasize = privdatasize;
    np->privdata = privdata;
    np->extradatasize = extradatasize;
    np->extradata = extradata;
    np->flags = flags;
    np->extraprofileflags = extraprofileflags;
    np->inDatabase = inDatabase;
    return auto_ptr<Stream>(np);
}
std::ostream & operator<<(std::ostream& os, Stream o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.streamindex.name() << " = " << o.streamindex << std::endl;
    os << o.streamtype.name() << " = " << o.streamtype << std::endl;
    os << o.codecid.name() << " = " << o.codecid << std::endl;
    os << o.codecname.name() << " = " << o.codecname << std::endl;
    os << o.frameratenum.name() << " = " << o.frameratenum << std::endl;
    os << o.framerateden.name() << " = " << o.framerateden << std::endl;
    os << o.streamtimebasenum.name() << " = " << o.streamtimebasenum << std::endl;
    os << o.streamtimebaseden.name() << " = " << o.streamtimebaseden << std::endl;
    os << o.codectimebasenum.name() << " = " << o.codectimebasenum << std::endl;
    os << o.codectimebaseden.name() << " = " << o.codectimebaseden << std::endl;
    os << o.firstpts.name() << " = " << o.firstpts << std::endl;
    os << o.firstdts.name() << " = " << o.firstdts << std::endl;
    os << o.duration.name() << " = " << o.duration << std::endl;
    os << o.nbframes.name() << " = " << o.nbframes << std::endl;
    os << o.ticksperframe.name() << " = " << o.ticksperframe << std::endl;
    os << o.framecount.name() << " = " << o.framecount << std::endl;
    os << o.width.name() << " = " << o.width << std::endl;
    os << o.height.name() << " = " << o.height << std::endl;
    os << o.gopsize.name() << " = " << o.gopsize << std::endl;
    os << o.pixfmt.name() << " = " << o.pixfmt << std::endl;
    os << o.bitrate.name() << " = " << o.bitrate << std::endl;
    os << o.samplerate.name() << " = " << o.samplerate << std::endl;
    os << o.samplefmt.name() << " = " << o.samplefmt << std::endl;
    os << o.channels.name() << " = " << o.channels << std::endl;
    os << o.bitspercodedsample.name() << " = " << o.bitspercodedsample << std::endl;
    os << o.privdatasize.name() << " = " << o.privdatasize << std::endl;
    os << o.privdata.name() << " = " << o.privdata << std::endl;
    os << o.extradatasize.name() << " = " << o.extradatasize << std::endl;
    os << o.extradata.name() << " = " << o.extradata << std::endl;
    os << o.flags.name() << " = " << o.flags << std::endl;
    os << o.extraprofileflags.name() << " = " << o.extraprofileflags << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType CodecPreset::Own::Id("id_","INTEGER","CodecPreset_");
const std::string CodecPreset::type__("CodecPreset");
const std::string CodecPreset::table__("CodecPreset_");
const std::string CodecPreset::sequence__("CodecPreset_seq");
const litesql::FieldType CodecPreset::Id("id_","INTEGER",table__);
const litesql::FieldType CodecPreset::Type("type_","TEXT",table__);
const litesql::FieldType CodecPreset::Name("name_","TEXT",table__);
const litesql::FieldType CodecPreset::Created("created_","INTEGER",table__);
const litesql::FieldType CodecPreset::Codecid("codecid_","INTEGER",table__);
const litesql::FieldType CodecPreset::Preset("preset_","TEXT",table__);
void CodecPreset::defaults() {
    id = 0;
    created = 0;
    codecid = 0;
}
CodecPreset::CodecPreset(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name), created(Created), codecid(Codecid), preset(Preset) {
    defaults();
}
CodecPreset::CodecPreset(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name), created(Created), codecid(Codecid), preset(Preset) {
    defaults();
    size_t size = (rec.size() > 6) ? 6 : rec.size();
    switch(size) {
    case 6: preset = convert<const std::string&, std::string>(rec[5]);
        preset.setModified(false);
    case 5: codecid = convert<const std::string&, int>(rec[4]);
        codecid.setModified(false);
    case 4: created = convert<const std::string&, litesql::Date>(rec[3]);
        created.setModified(false);
    case 3: name = convert<const std::string&, std::string>(rec[2]);
        name.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
CodecPreset::CodecPreset(const CodecPreset& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name), created(obj.created), codecid(obj.codecid), preset(obj.preset) {
}
const CodecPreset& CodecPreset::operator=(const CodecPreset& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
        created = obj.created;
        codecid = obj.codecid;
        preset = obj.preset;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string CodecPreset::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(name.name());
    values.push_back(name);
    name.setModified(false);
    fields.push_back(created.name());
    values.push_back(created);
    created.setModified(false);
    fields.push_back(codecid.name());
    values.push_back(codecid);
    codecid.setModified(false);
    fields.push_back(preset.name());
    values.push_back(preset);
    preset.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void CodecPreset::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void CodecPreset::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, name);
    updateField(updates, table__, created);
    updateField(updates, table__, codecid);
    updateField(updates, table__, preset);
}
void CodecPreset::addIDUpdates(Updates& updates) {
}
void CodecPreset::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
    ftypes.push_back(Created);
    ftypes.push_back(Codecid);
    ftypes.push_back(Preset);
}
void CodecPreset::delRecord() {
    deleteFromTable(table__, id);
}
void CodecPreset::delRelations() {
}
void CodecPreset::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void CodecPreset::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<CodecPreset> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool CodecPreset::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<CodecPreset> CodecPreset::upcast() {
    return auto_ptr<CodecPreset>(new CodecPreset(*this));
}
std::auto_ptr<CodecPreset> CodecPreset::upcastCopy() {
    CodecPreset* np = new CodecPreset(*this);
    np->id = id;
    np->type = type;
    np->name = name;
    np->created = created;
    np->codecid = codecid;
    np->preset = preset;
    np->inDatabase = inDatabase;
    return auto_ptr<CodecPreset>(np);
}
std::ostream & operator<<(std::ostream& os, CodecPreset o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.created.name() << " = " << o.created << std::endl;
    os << o.codecid.name() << " = " << o.codecid << std::endl;
    os << o.preset.name() << " = " << o.preset << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Config::Own::Id("id_","INTEGER","Config_");
const std::string Config::type__("Config");
const std::string Config::table__("Config_");
const std::string Config::sequence__("Config_seq");
const litesql::FieldType Config::Id("id_","INTEGER",table__);
const litesql::FieldType Config::Type("type_","TEXT",table__);
const litesql::FieldType Config::Configkey("configkey_","TEXT",table__);
const litesql::FieldType Config::Configval("configval_","TEXT",table__);
void Config::defaults() {
    id = 0;
}
Config::Config(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), configkey(Configkey), configval(Configval) {
    defaults();
}
Config::Config(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), configkey(Configkey), configval(Configval) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: configval = convert<const std::string&, std::string>(rec[3]);
        configval.setModified(false);
    case 3: configkey = convert<const std::string&, std::string>(rec[2]);
        configkey.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Config::Config(const Config& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), configkey(obj.configkey), configval(obj.configval) {
}
const Config& Config::operator=(const Config& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        configkey = obj.configkey;
        configval = obj.configval;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string Config::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(configkey.name());
    values.push_back(configkey);
    configkey.setModified(false);
    fields.push_back(configval.name());
    values.push_back(configval);
    configval.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Config::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Config::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, configkey);
    updateField(updates, table__, configval);
}
void Config::addIDUpdates(Updates& updates) {
}
void Config::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Configkey);
    ftypes.push_back(Configval);
}
void Config::delRecord() {
    deleteFromTable(table__, id);
}
void Config::delRelations() {
}
void Config::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void Config::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<Config> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Config::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<Config> Config::upcast() {
    return auto_ptr<Config>(new Config(*this));
}
std::auto_ptr<Config> Config::upcastCopy() {
    Config* np = new Config(*this);
    np->id = id;
    np->type = type;
    np->configkey = configkey;
    np->configval = configval;
    np->inDatabase = inDatabase;
    return auto_ptr<Config>(np);
}
std::ostream & operator<<(std::ostream& os, Config o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.configkey.name() << " = " << o.configkey << std::endl;
    os << o.configval.name() << " = " << o.configval << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Job::Own::Id("id_","INTEGER","Job_");
Job::InputfileHandle::InputfileHandle(const Job& owner)
         : litesql::RelationHandle<Job>(owner) {
}
void Job::InputfileHandle::link(const MediaFile& o0) {
    JobMediaFileRelationJobInFile::link(owner->getDatabase(), *owner, o0);
}
void Job::InputfileHandle::unlink(const MediaFile& o0) {
    JobMediaFileRelationJobInFile::unlink(owner->getDatabase(), *owner, o0);
}
void Job::InputfileHandle::del(const litesql::Expr& expr) {
    JobMediaFileRelationJobInFile::del(owner->getDatabase(), expr && JobMediaFileRelationJobInFile::Job == owner->id);
}
litesql::DataSource<MediaFile> Job::InputfileHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobMediaFileRelationJobInFile::get<MediaFile>(owner->getDatabase(), expr, (JobMediaFileRelationJobInFile::Job == owner->id) && srcExpr);
}
litesql::DataSource<JobMediaFileRelationJobInFile::Row> Job::InputfileHandle::getRows(const litesql::Expr& expr) {
    return JobMediaFileRelationJobInFile::getRows(owner->getDatabase(), expr && (JobMediaFileRelationJobInFile::Job == owner->id));
}
Job::OutputfileHandle::OutputfileHandle(const Job& owner)
         : litesql::RelationHandle<Job>(owner) {
}
void Job::OutputfileHandle::link(const MediaFile& o0) {
    JobMediaFileRelationJobOutFile::link(owner->getDatabase(), *owner, o0);
}
void Job::OutputfileHandle::unlink(const MediaFile& o0) {
    JobMediaFileRelationJobOutFile::unlink(owner->getDatabase(), *owner, o0);
}
void Job::OutputfileHandle::del(const litesql::Expr& expr) {
    JobMediaFileRelationJobOutFile::del(owner->getDatabase(), expr && JobMediaFileRelationJobOutFile::Job == owner->id);
}
litesql::DataSource<MediaFile> Job::OutputfileHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobMediaFileRelationJobOutFile::get<MediaFile>(owner->getDatabase(), expr, (JobMediaFileRelationJobOutFile::Job == owner->id) && srcExpr);
}
litesql::DataSource<JobMediaFileRelationJobOutFile::Row> Job::OutputfileHandle::getRows(const litesql::Expr& expr) {
    return JobMediaFileRelationJobOutFile::getRows(owner->getDatabase(), expr && (JobMediaFileRelationJobOutFile::Job == owner->id));
}
Job::JobdetailsHandle::JobdetailsHandle(const Job& owner)
         : litesql::RelationHandle<Job>(owner) {
}
void Job::JobdetailsHandle::link(const JobDetail& o0) {
    JobJobDetailRelationJobJobDetail::link(owner->getDatabase(), *owner, o0);
}
void Job::JobdetailsHandle::unlink(const JobDetail& o0) {
    JobJobDetailRelationJobJobDetail::unlink(owner->getDatabase(), *owner, o0);
}
void Job::JobdetailsHandle::del(const litesql::Expr& expr) {
    JobJobDetailRelationJobJobDetail::del(owner->getDatabase(), expr && JobJobDetailRelationJobJobDetail::Job == owner->id);
}
litesql::DataSource<JobDetail> Job::JobdetailsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobJobDetailRelationJobJobDetail::get<JobDetail>(owner->getDatabase(), expr, (JobJobDetailRelationJobJobDetail::Job == owner->id) && srcExpr);
}
litesql::DataSource<JobJobDetailRelationJobJobDetail::Row> Job::JobdetailsHandle::getRows(const litesql::Expr& expr) {
    return JobJobDetailRelationJobJobDetail::getRows(owner->getDatabase(), expr && (JobJobDetailRelationJobJobDetail::Job == owner->id));
}
const std::string Job::type__("Job");
const std::string Job::table__("Job_");
const std::string Job::sequence__("Job_seq");
const litesql::FieldType Job::Id("id_","INTEGER",table__);
const litesql::FieldType Job::Type("type_","TEXT",table__);
const litesql::FieldType Job::Begintime("begintime_","INTEGER",table__);
const litesql::FieldType Job::Endtime("endtime_","INTEGER",table__);
const litesql::FieldType Job::Status("status_","TEXT",table__);
void Job::defaults() {
    id = 0;
    begintime = -1;
    endtime = -1;
}
Job::Job(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), begintime(Begintime), endtime(Endtime), status(Status) {
    defaults();
}
Job::Job(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), begintime(Begintime), endtime(Endtime), status(Status) {
    defaults();
    size_t size = (rec.size() > 5) ? 5 : rec.size();
    switch(size) {
    case 5: status = convert<const std::string&, std::string>(rec[4]);
        status.setModified(false);
    case 4: endtime = convert<const std::string&, litesql::DateTime>(rec[3]);
        endtime.setModified(false);
    case 3: begintime = convert<const std::string&, litesql::DateTime>(rec[2]);
        begintime.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Job::Job(const Job& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), begintime(obj.begintime), endtime(obj.endtime), status(obj.status) {
}
const Job& Job::operator=(const Job& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        begintime = obj.begintime;
        endtime = obj.endtime;
        status = obj.status;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Job::InputfileHandle Job::inputfile() {
    return Job::InputfileHandle(*this);
}
Job::OutputfileHandle Job::outputfile() {
    return Job::OutputfileHandle(*this);
}
Job::JobdetailsHandle Job::jobdetails() {
    return Job::JobdetailsHandle(*this);
}
std::string Job::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(begintime.name());
    values.push_back(begintime);
    begintime.setModified(false);
    fields.push_back(endtime.name());
    values.push_back(endtime);
    endtime.setModified(false);
    fields.push_back(status.name());
    values.push_back(status);
    status.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Job::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Job::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, begintime);
    updateField(updates, table__, endtime);
    updateField(updates, table__, status);
}
void Job::addIDUpdates(Updates& updates) {
}
void Job::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Begintime);
    ftypes.push_back(Endtime);
    ftypes.push_back(Status);
}
void Job::delRecord() {
    deleteFromTable(table__, id);
}
void Job::delRelations() {
    JobMediaFileRelationJobInFile::del(*db, (JobMediaFileRelationJobInFile::Job == id));
    JobMediaFileRelationJobOutFile::del(*db, (JobMediaFileRelationJobOutFile::Job == id));
    JobJobDetailRelationJobJobDetail::del(*db, (JobJobDetailRelationJobJobDetail::Job == id));
}
void Job::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void Job::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<Job> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Job::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<Job> Job::upcast() {
    return auto_ptr<Job>(new Job(*this));
}
std::auto_ptr<Job> Job::upcastCopy() {
    Job* np = new Job(*this);
    np->id = id;
    np->type = type;
    np->begintime = begintime;
    np->endtime = endtime;
    np->status = status;
    np->inDatabase = inDatabase;
    return auto_ptr<Job>(np);
}
std::ostream & operator<<(std::ostream& os, Job o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.begintime.name() << " = " << o.begintime << std::endl;
    os << o.endtime.name() << " = " << o.endtime << std::endl;
    os << o.status.name() << " = " << o.status << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType JobDetail::Own::Id("id_","INTEGER","JobDetail_");
JobDetail::JobHandle::JobHandle(const JobDetail& owner)
         : litesql::RelationHandle<JobDetail>(owner) {
}
void JobDetail::JobHandle::link(const Job& o0) {
    JobJobDetailRelationJobJobDetail::link(owner->getDatabase(), o0, *owner);
}
void JobDetail::JobHandle::unlink(const Job& o0) {
    JobJobDetailRelationJobJobDetail::unlink(owner->getDatabase(), o0, *owner);
}
void JobDetail::JobHandle::del(const litesql::Expr& expr) {
    JobJobDetailRelationJobJobDetail::del(owner->getDatabase(), expr && JobJobDetailRelationJobJobDetail::JobDetail == owner->id);
}
litesql::DataSource<Job> JobDetail::JobHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobJobDetailRelationJobJobDetail::get<Job>(owner->getDatabase(), expr, (JobJobDetailRelationJobJobDetail::JobDetail == owner->id) && srcExpr);
}
litesql::DataSource<JobJobDetailRelationJobJobDetail::Row> JobDetail::JobHandle::getRows(const litesql::Expr& expr) {
    return JobJobDetailRelationJobJobDetail::getRows(owner->getDatabase(), expr && (JobJobDetailRelationJobJobDetail::JobDetail == owner->id));
}
JobDetail::OutputstreamHandle::OutputstreamHandle(const JobDetail& owner)
         : litesql::RelationHandle<JobDetail>(owner) {
}
void JobDetail::OutputstreamHandle::link(const Stream& o0) {
    JobDetailStreamRelationJobOutStream::link(owner->getDatabase(), *owner, o0);
}
void JobDetail::OutputstreamHandle::unlink(const Stream& o0) {
    JobDetailStreamRelationJobOutStream::unlink(owner->getDatabase(), *owner, o0);
}
void JobDetail::OutputstreamHandle::del(const litesql::Expr& expr) {
    JobDetailStreamRelationJobOutStream::del(owner->getDatabase(), expr && JobDetailStreamRelationJobOutStream::JobDetail == owner->id);
}
litesql::DataSource<Stream> JobDetail::OutputstreamHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobDetailStreamRelationJobOutStream::get<Stream>(owner->getDatabase(), expr, (JobDetailStreamRelationJobOutStream::JobDetail == owner->id) && srcExpr);
}
litesql::DataSource<JobDetailStreamRelationJobOutStream::Row> JobDetail::OutputstreamHandle::getRows(const litesql::Expr& expr) {
    return JobDetailStreamRelationJobOutStream::getRows(owner->getDatabase(), expr && (JobDetailStreamRelationJobOutStream::JobDetail == owner->id));
}
JobDetail::InputstreamHandle::InputstreamHandle(const JobDetail& owner)
         : litesql::RelationHandle<JobDetail>(owner) {
}
void JobDetail::InputstreamHandle::link(const Stream& o0) {
    JobDetailStreamRelationJobInStream::link(owner->getDatabase(), *owner, o0);
}
void JobDetail::InputstreamHandle::unlink(const Stream& o0) {
    JobDetailStreamRelationJobInStream::unlink(owner->getDatabase(), *owner, o0);
}
void JobDetail::InputstreamHandle::del(const litesql::Expr& expr) {
    JobDetailStreamRelationJobInStream::del(owner->getDatabase(), expr && JobDetailStreamRelationJobInStream::JobDetail == owner->id);
}
litesql::DataSource<Stream> JobDetail::InputstreamHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobDetailStreamRelationJobInStream::get<Stream>(owner->getDatabase(), expr, (JobDetailStreamRelationJobInStream::JobDetail == owner->id) && srcExpr);
}
litesql::DataSource<JobDetailStreamRelationJobInStream::Row> JobDetail::InputstreamHandle::getRows(const litesql::Expr& expr) {
    return JobDetailStreamRelationJobInStream::getRows(owner->getDatabase(), expr && (JobDetailStreamRelationJobInStream::JobDetail == owner->id));
}
const std::string JobDetail::type__("JobDetail");
const std::string JobDetail::table__("JobDetail_");
const std::string JobDetail::sequence__("JobDetail_seq");
const litesql::FieldType JobDetail::Id("id_","INTEGER",table__);
const litesql::FieldType JobDetail::Type("type_","TEXT",table__);
const litesql::FieldType JobDetail::Lastpts("lastpts_","DOUBLE",table__);
const litesql::FieldType JobDetail::Lastdts("lastdts_","DOUBLE",table__);
void JobDetail::defaults() {
    id = 0;
    lastpts = 0.0;
    lastdts = 0.0;
}
JobDetail::JobDetail(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), lastpts(Lastpts), lastdts(Lastdts) {
    defaults();
}
JobDetail::JobDetail(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), lastpts(Lastpts), lastdts(Lastdts) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: lastdts = convert<const std::string&, double>(rec[3]);
        lastdts.setModified(false);
    case 3: lastpts = convert<const std::string&, double>(rec[2]);
        lastpts.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
JobDetail::JobDetail(const JobDetail& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), lastpts(obj.lastpts), lastdts(obj.lastdts) {
}
const JobDetail& JobDetail::operator=(const JobDetail& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        lastpts = obj.lastpts;
        lastdts = obj.lastdts;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
JobDetail::JobHandle JobDetail::job() {
    return JobDetail::JobHandle(*this);
}
JobDetail::OutputstreamHandle JobDetail::outputstream() {
    return JobDetail::OutputstreamHandle(*this);
}
JobDetail::InputstreamHandle JobDetail::inputstream() {
    return JobDetail::InputstreamHandle(*this);
}
std::string JobDetail::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(lastpts.name());
    values.push_back(lastpts);
    lastpts.setModified(false);
    fields.push_back(lastdts.name());
    values.push_back(lastdts);
    lastdts.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void JobDetail::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void JobDetail::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, lastpts);
    updateField(updates, table__, lastdts);
}
void JobDetail::addIDUpdates(Updates& updates) {
}
void JobDetail::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Lastpts);
    ftypes.push_back(Lastdts);
}
void JobDetail::delRecord() {
    deleteFromTable(table__, id);
}
void JobDetail::delRelations() {
    JobJobDetailRelationJobJobDetail::del(*db, (JobJobDetailRelationJobJobDetail::JobDetail == id));
    JobDetailStreamRelationJobOutStream::del(*db, (JobDetailStreamRelationJobOutStream::JobDetail == id));
    JobDetailStreamRelationJobInStream::del(*db, (JobDetailStreamRelationJobInStream::JobDetail == id));
}
void JobDetail::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void JobDetail::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<JobDetail> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool JobDetail::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<JobDetail> JobDetail::upcast() {
    return auto_ptr<JobDetail>(new JobDetail(*this));
}
std::auto_ptr<JobDetail> JobDetail::upcastCopy() {
    JobDetail* np = new JobDetail(*this);
    np->id = id;
    np->type = type;
    np->lastpts = lastpts;
    np->lastdts = lastdts;
    np->inDatabase = inDatabase;
    return auto_ptr<JobDetail>(np);
}
std::ostream & operator<<(std::ostream& os, JobDetail o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.lastpts.name() << " = " << o.lastpts << std::endl;
    os << o.lastdts.name() << " = " << o.lastdts << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Watchfolder::Own::Id("id_","INTEGER","Watchfolder_");
Watchfolder::ProfileHandle::ProfileHandle(const Watchfolder& owner)
         : litesql::RelationHandle<Watchfolder>(owner) {
}
void Watchfolder::ProfileHandle::link(const Profile& o0) {
    ProfileWatchfolderRelationWatchfolderProfile::link(owner->getDatabase(), o0, *owner);
}
void Watchfolder::ProfileHandle::unlink(const Profile& o0) {
    ProfileWatchfolderRelationWatchfolderProfile::unlink(owner->getDatabase(), o0, *owner);
}
void Watchfolder::ProfileHandle::del(const litesql::Expr& expr) {
    ProfileWatchfolderRelationWatchfolderProfile::del(owner->getDatabase(), expr && ProfileWatchfolderRelationWatchfolderProfile::Watchfolder == owner->id);
}
litesql::DataSource<Profile> Watchfolder::ProfileHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return ProfileWatchfolderRelationWatchfolderProfile::get<Profile>(owner->getDatabase(), expr, (ProfileWatchfolderRelationWatchfolderProfile::Watchfolder == owner->id) && srcExpr);
}
litesql::DataSource<ProfileWatchfolderRelationWatchfolderProfile::Row> Watchfolder::ProfileHandle::getRows(const litesql::Expr& expr) {
    return ProfileWatchfolderRelationWatchfolderProfile::getRows(owner->getDatabase(), expr && (ProfileWatchfolderRelationWatchfolderProfile::Watchfolder == owner->id));
}
const std::string Watchfolder::type__("Watchfolder");
const std::string Watchfolder::table__("Watchfolder_");
const std::string Watchfolder::sequence__("Watchfolder_seq");
const litesql::FieldType Watchfolder::Id("id_","INTEGER",table__);
const litesql::FieldType Watchfolder::Type("type_","TEXT",table__);
const litesql::FieldType Watchfolder::Infolder("infolder_","TEXT",table__);
const litesql::FieldType Watchfolder::Outfolder("outfolder_","TEXT",table__);
const litesql::FieldType Watchfolder::Extensionfilter("extensionfilter_","TEXT",table__);
void Watchfolder::defaults() {
    id = 0;
}
Watchfolder::Watchfolder(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), infolder(Infolder), outfolder(Outfolder), extensionfilter(Extensionfilter) {
    defaults();
}
Watchfolder::Watchfolder(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), infolder(Infolder), outfolder(Outfolder), extensionfilter(Extensionfilter) {
    defaults();
    size_t size = (rec.size() > 5) ? 5 : rec.size();
    switch(size) {
    case 5: extensionfilter = convert<const std::string&, std::string>(rec[4]);
        extensionfilter.setModified(false);
    case 4: outfolder = convert<const std::string&, std::string>(rec[3]);
        outfolder.setModified(false);
    case 3: infolder = convert<const std::string&, std::string>(rec[2]);
        infolder.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Watchfolder::Watchfolder(const Watchfolder& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), infolder(obj.infolder), outfolder(obj.outfolder), extensionfilter(obj.extensionfilter) {
}
const Watchfolder& Watchfolder::operator=(const Watchfolder& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        infolder = obj.infolder;
        outfolder = obj.outfolder;
        extensionfilter = obj.extensionfilter;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Watchfolder::ProfileHandle Watchfolder::profile() {
    return Watchfolder::ProfileHandle(*this);
}
std::string Watchfolder::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(infolder.name());
    values.push_back(infolder);
    infolder.setModified(false);
    fields.push_back(outfolder.name());
    values.push_back(outfolder);
    outfolder.setModified(false);
    fields.push_back(extensionfilter.name());
    values.push_back(extensionfilter);
    extensionfilter.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Watchfolder::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Watchfolder::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, infolder);
    updateField(updates, table__, outfolder);
    updateField(updates, table__, extensionfilter);
}
void Watchfolder::addIDUpdates(Updates& updates) {
}
void Watchfolder::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Infolder);
    ftypes.push_back(Outfolder);
    ftypes.push_back(Extensionfilter);
}
void Watchfolder::delRecord() {
    deleteFromTable(table__, id);
}
void Watchfolder::delRelations() {
    ProfileWatchfolderRelationWatchfolderProfile::del(*db, (ProfileWatchfolderRelationWatchfolderProfile::Watchfolder == id));
}
void Watchfolder::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void Watchfolder::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<Watchfolder> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Watchfolder::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<Watchfolder> Watchfolder::upcast() {
    return auto_ptr<Watchfolder>(new Watchfolder(*this));
}
std::auto_ptr<Watchfolder> Watchfolder::upcastCopy() {
    Watchfolder* np = new Watchfolder(*this);
    np->id = id;
    np->type = type;
    np->infolder = infolder;
    np->outfolder = outfolder;
    np->extensionfilter = extensionfilter;
    np->inDatabase = inDatabase;
    return auto_ptr<Watchfolder>(np);
}
std::ostream & operator<<(std::ostream& os, Watchfolder o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.infolder.name() << " = " << o.infolder << std::endl;
    os << o.outfolder.name() << " = " << o.outfolder << std::endl;
    os << o.extensionfilter.name() << " = " << o.extensionfilter << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType ProcessUnit::Own::Id("id_","INTEGER","ProcessUnit_");
const std::string ProcessUnit::type__("ProcessUnit");
const std::string ProcessUnit::table__("ProcessUnit_");
const std::string ProcessUnit::sequence__("ProcessUnit_seq");
const litesql::FieldType ProcessUnit::Id("id_","INTEGER",table__);
const litesql::FieldType ProcessUnit::Type("type_","TEXT",table__);
const litesql::FieldType ProcessUnit::Sorcestream("sorcestream_","INTEGER",table__);
const litesql::FieldType ProcessUnit::Targetstream("targetstream_","INTEGER",table__);
const litesql::FieldType ProcessUnit::Startts("startts_","DOUBLE",table__);
const litesql::FieldType ProcessUnit::Endts("endts_","DOUBLE",table__);
const litesql::FieldType ProcessUnit::Framecount("framecount_","INTEGER",table__);
const litesql::FieldType ProcessUnit::Send("send_","INTEGER",table__);
const litesql::FieldType ProcessUnit::Recv("recv_","INTEGER",table__);
void ProcessUnit::defaults() {
    id = 0;
    sorcestream = 0;
    targetstream = 0;
    startts = 0.0;
    endts = 0.0;
    framecount = 0;
    send = 0;
    recv = 0;
}
ProcessUnit::ProcessUnit(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), sorcestream(Sorcestream), targetstream(Targetstream), startts(Startts), endts(Endts), framecount(Framecount), send(Send), recv(Recv) {
    defaults();
}
ProcessUnit::ProcessUnit(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), sorcestream(Sorcestream), targetstream(Targetstream), startts(Startts), endts(Endts), framecount(Framecount), send(Send), recv(Recv) {
    defaults();
    size_t size = (rec.size() > 9) ? 9 : rec.size();
    switch(size) {
    case 9: recv = convert<const std::string&, litesql::DateTime>(rec[8]);
        recv.setModified(false);
    case 8: send = convert<const std::string&, litesql::DateTime>(rec[7]);
        send.setModified(false);
    case 7: framecount = convert<const std::string&, int>(rec[6]);
        framecount.setModified(false);
    case 6: endts = convert<const std::string&, double>(rec[5]);
        endts.setModified(false);
    case 5: startts = convert<const std::string&, double>(rec[4]);
        startts.setModified(false);
    case 4: targetstream = convert<const std::string&, int>(rec[3]);
        targetstream.setModified(false);
    case 3: sorcestream = convert<const std::string&, int>(rec[2]);
        sorcestream.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
ProcessUnit::ProcessUnit(const ProcessUnit& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), sorcestream(obj.sorcestream), targetstream(obj.targetstream), startts(obj.startts), endts(obj.endts), framecount(obj.framecount), send(obj.send), recv(obj.recv) {
}
const ProcessUnit& ProcessUnit::operator=(const ProcessUnit& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        sorcestream = obj.sorcestream;
        targetstream = obj.targetstream;
        startts = obj.startts;
        endts = obj.endts;
        framecount = obj.framecount;
        send = obj.send;
        recv = obj.recv;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string ProcessUnit::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(sorcestream.name());
    values.push_back(sorcestream);
    sorcestream.setModified(false);
    fields.push_back(targetstream.name());
    values.push_back(targetstream);
    targetstream.setModified(false);
    fields.push_back(startts.name());
    values.push_back(startts);
    startts.setModified(false);
    fields.push_back(endts.name());
    values.push_back(endts);
    endts.setModified(false);
    fields.push_back(framecount.name());
    values.push_back(framecount);
    framecount.setModified(false);
    fields.push_back(send.name());
    values.push_back(send);
    send.setModified(false);
    fields.push_back(recv.name());
    values.push_back(recv);
    recv.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void ProcessUnit::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void ProcessUnit::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, sorcestream);
    updateField(updates, table__, targetstream);
    updateField(updates, table__, startts);
    updateField(updates, table__, endts);
    updateField(updates, table__, framecount);
    updateField(updates, table__, send);
    updateField(updates, table__, recv);
}
void ProcessUnit::addIDUpdates(Updates& updates) {
}
void ProcessUnit::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Sorcestream);
    ftypes.push_back(Targetstream);
    ftypes.push_back(Startts);
    ftypes.push_back(Endts);
    ftypes.push_back(Framecount);
    ftypes.push_back(Send);
    ftypes.push_back(Recv);
}
void ProcessUnit::delRecord() {
    deleteFromTable(table__, id);
}
void ProcessUnit::delRelations() {
}
void ProcessUnit::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void ProcessUnit::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<ProcessUnit> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool ProcessUnit::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<ProcessUnit> ProcessUnit::upcast() {
    return auto_ptr<ProcessUnit>(new ProcessUnit(*this));
}
std::auto_ptr<ProcessUnit> ProcessUnit::upcastCopy() {
    ProcessUnit* np = new ProcessUnit(*this);
    np->id = id;
    np->type = type;
    np->sorcestream = sorcestream;
    np->targetstream = targetstream;
    np->startts = startts;
    np->endts = endts;
    np->framecount = framecount;
    np->send = send;
    np->recv = recv;
    np->inDatabase = inDatabase;
    return auto_ptr<ProcessUnit>(np);
}
std::ostream & operator<<(std::ostream& os, ProcessUnit o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.sorcestream.name() << " = " << o.sorcestream << std::endl;
    os << o.targetstream.name() << " = " << o.targetstream << std::endl;
    os << o.startts.name() << " = " << o.startts << std::endl;
    os << o.endts.name() << " = " << o.endts << std::endl;
    os << o.framecount.name() << " = " << o.framecount << std::endl;
    os << o.send.name() << " = " << o.send << std::endl;
    os << o.recv.name() << " = " << o.recv << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
HiveDb::HiveDb(std::string backendType, std::string connInfo)
     : litesql::Database(backendType, connInfo) {
    initialize();
}
std::vector<litesql::Database::SchemaItem> HiveDb::getSchema() const {
    vector<Database::SchemaItem> res;
    res.push_back(Database::SchemaItem("schema_","table","CREATE TABLE schema_ (name_ TEXT, type_ TEXT, sql_ TEXT);"));
    if (backend->supportsSequences()) {
        res.push_back(Database::SchemaItem("Project_seq","sequence","CREATE SEQUENCE Project_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("Filter_seq","sequence","CREATE SEQUENCE Filter_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("FilterParameter_seq","sequence","CREATE SEQUENCE FilterParameter_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("MediaFile_seq","sequence","CREATE SEQUENCE MediaFile_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("Profile_seq","sequence","CREATE SEQUENCE Profile_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("Stream_seq","sequence","CREATE SEQUENCE Stream_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("CodecPreset_seq","sequence","CREATE SEQUENCE CodecPreset_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("Config_seq","sequence","CREATE SEQUENCE Config_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("Job_seq","sequence","CREATE SEQUENCE Job_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("JobDetail_seq","sequence","CREATE SEQUENCE JobDetail_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("Watchfolder_seq","sequence","CREATE SEQUENCE Watchfolder_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("ProcessUnit_seq","sequence","CREATE SEQUENCE ProcessUnit_seq START 1 INCREMENT 1"));
    }
    res.push_back(Database::SchemaItem("Project_","table","CREATE TABLE Project_ (id_ " + backend->getRowIDType() + ",type_ TEXT,name_ TEXT,created_ INTEGER)"));
    res.push_back(Database::SchemaItem("Filter_","table","CREATE TABLE Filter_ (id_ " + backend->getRowIDType() + ",type_ TEXT,filtername_ TEXT,filterid_ TEXT)"));
    res.push_back(Database::SchemaItem("FilterParameter_","table","CREATE TABLE FilterParameter_ (id_ " + backend->getRowIDType() + ",type_ TEXT,fkey_ TEXT,fval_ TEXT)"));
    res.push_back(Database::SchemaItem("MediaFile_","table","CREATE TABLE MediaFile_ (id_ " + backend->getRowIDType() + ",type_ TEXT,filename_ TEXT,path_ TEXT,filesize_ DOUBLE,streamcount_ INTEGER,containertype_ TEXT,duration_ DOUBLE,bitrate_ INTEGER,created_ INTEGER,filetype_ INTEGER,parent_ INTEGER,metatitle_ TEXT,metaauthor_ TEXT,metacopyright_ TEXT,metacomment_ TEXT,metaalbum_ TEXT,metayear_ INTEGER,metatrack_ INTEGER,metagenre_ INTEGER)"));
    res.push_back(Database::SchemaItem("Profile_","table","CREATE TABLE Profile_ (id_ " + backend->getRowIDType() + ",type_ TEXT,name_ TEXT,created_ INTEGER,format_ TEXT,formatext_ TEXT,vcodec_ INTEGER,vbitrate_ INTEGER,vframerate_ TEXT,vwidth_ INTEGER,vheight_ INTEGER,vextra_ TEXT,achannels_ INTEGER,acodec_ INTEGER,abitrate_ INTEGER,asamplerate_ INTEGER,aextra_ TEXT,profiletype_ INTEGER)"));
    res.push_back(Database::SchemaItem("Stream_","table","CREATE TABLE Stream_ (id_ " + backend->getRowIDType() + ",type_ TEXT,streamindex_ INTEGER,streamtype_ INTEGER,codecid_ INTEGER,codecname_ TEXT,frameratenum_ INTEGER,framerateden_ INTEGER,streamtimebasenum_ INTEGER,streamtimebaseden_ INTEGER,codectimebasenum_ INTEGER,codectimebaseden_ INTEGER,firstpts_ DOUBLE,firstdts_ DOUBLE,duration_ DOUBLE,nbframes_ DOUBLE,ticksperframe_ INTEGER,framecount_ INTEGER,width_ INTEGER,height_ INTEGER,gopsize_ INTEGER,pixfmt_ INTEGER,bitrate_ INTEGER,samplerate_ INTEGER,samplefmt_ INTEGER,channels_ INTEGER,bitspercodedsample_ INTEGER,privdatasize_ INTEGER,privdata_ TEXT,extradatasize_ INTEGER,extradata_ TEXT,flags_ INTEGER,extraprofileflags_ TEXT)"));
    res.push_back(Database::SchemaItem("CodecPreset_","table","CREATE TABLE CodecPreset_ (id_ " + backend->getRowIDType() + ",type_ TEXT,name_ TEXT,created_ INTEGER,codecid_ INTEGER,preset_ TEXT)"));
    res.push_back(Database::SchemaItem("Config_","table","CREATE TABLE Config_ (id_ " + backend->getRowIDType() + ",type_ TEXT,configkey_ TEXT,configval_ TEXT)"));
    res.push_back(Database::SchemaItem("Job_","table","CREATE TABLE Job_ (id_ " + backend->getRowIDType() + ",type_ TEXT,begintime_ INTEGER,endtime_ INTEGER,status_ TEXT)"));
    res.push_back(Database::SchemaItem("JobDetail_","table","CREATE TABLE JobDetail_ (id_ " + backend->getRowIDType() + ",type_ TEXT,lastpts_ DOUBLE,lastdts_ DOUBLE)"));
    res.push_back(Database::SchemaItem("Watchfolder_","table","CREATE TABLE Watchfolder_ (id_ " + backend->getRowIDType() + ",type_ TEXT,infolder_ TEXT,outfolder_ TEXT,extensionfilter_ TEXT)"));
    res.push_back(Database::SchemaItem("ProcessUnit_","table","CREATE TABLE ProcessUnit_ (id_ " + backend->getRowIDType() + ",type_ TEXT,sorcestream_ INTEGER,targetstream_ INTEGER,startts_ DOUBLE,endts_ DOUBLE,framecount_ INTEGER,send_ INTEGER,recv_ INTEGER)"));
    res.push_back(Database::SchemaItem("Filter_FilterParameter_","table","CREATE TABLE Filter_FilterParameter_ (Filter1 INTEGER,FilterParameter2 INTEGER)"));
    res.push_back(Database::SchemaItem("MediaFile_Project_","table","CREATE TABLE MediaFile_Project_ (MediaFile1 INTEGER UNIQUE,Project2 INTEGER)"));
    res.push_back(Database::SchemaItem("Profile_Project_","table","CREATE TABLE Profile_Project_ (Profile1 INTEGER,Project2 INTEGER)"));
    res.push_back(Database::SchemaItem("MediaFile_Stream_","table","CREATE TABLE MediaFile_Stream_ (MediaFile1 INTEGER,Stream2 INTEGER UNIQUE)"));
    res.push_back(Database::SchemaItem("Job_MediaFile_JobInFile","table","CREATE TABLE Job_MediaFile_JobInFile (Job1 INTEGER,MediaFile2 INTEGER)"));
    res.push_back(Database::SchemaItem("Job_MediaFile_JobOutFile","table","CREATE TABLE Job_MediaFile_JobOutFile (Job1 INTEGER,MediaFile2 INTEGER)"));
    res.push_back(Database::SchemaItem("Job_JobDetail_JobJobDetail","table","CREATE TABLE Job_JobDetail_JobJobDetail (Job1 INTEGER,JobDetail2 INTEGER)"));
    res.push_back(Database::SchemaItem("JobDetail_Stream_JobOutStream","table","CREATE TABLE JobDetail_Stream_JobOutStream (JobDetail1 INTEGER,Stream2 INTEGER)"));
    res.push_back(Database::SchemaItem("JobDetail_Stream_JobInStream","table","CREATE TABLE JobDetail_Stream_JobInStream (JobDetail1 INTEGER,Stream2 INTEGER)"));
    res.push_back(Database::SchemaItem("_72915fab98e40e57ddd1495ecd15b95b","table","CREATE TABLE _72915fab98e40e57ddd1495ecd15b95b (Profile1 INTEGER,Watchfolder2 INTEGER)"));
    res.push_back(Database::SchemaItem("_864f17f6c9c6e1560a3b610198ace17e","index","CREATE INDEX _864f17f6c9c6e1560a3b610198ace17e ON Filter_FilterParameter_ (Filter1)"));
    res.push_back(Database::SchemaItem("_ebb60e0eabfba5df99ab088688ea3579","index","CREATE INDEX _ebb60e0eabfba5df99ab088688ea3579 ON Filter_FilterParameter_ (FilterParameter2)"));
    res.push_back(Database::SchemaItem("Filter_FilterParameter__all_idx","index","CREATE INDEX Filter_FilterParameter__all_idx ON Filter_FilterParameter_ (Filter1,FilterParameter2)"));
    res.push_back(Database::SchemaItem("MediaFile_Project_MediaFile1idx","index","CREATE INDEX MediaFile_Project_MediaFile1idx ON MediaFile_Project_ (MediaFile1)"));
    res.push_back(Database::SchemaItem("MediaFile_Project_Project2idx","index","CREATE INDEX MediaFile_Project_Project2idx ON MediaFile_Project_ (Project2)"));
    res.push_back(Database::SchemaItem("MediaFile_Project__all_idx","index","CREATE INDEX MediaFile_Project__all_idx ON MediaFile_Project_ (MediaFile1,Project2)"));
    res.push_back(Database::SchemaItem("Profile_Project_Profile1idx","index","CREATE INDEX Profile_Project_Profile1idx ON Profile_Project_ (Profile1)"));
    res.push_back(Database::SchemaItem("Profile_Project_Project2idx","index","CREATE INDEX Profile_Project_Project2idx ON Profile_Project_ (Project2)"));
    res.push_back(Database::SchemaItem("Profile_Project__all_idx","index","CREATE INDEX Profile_Project__all_idx ON Profile_Project_ (Profile1,Project2)"));
    res.push_back(Database::SchemaItem("MediaFile_Stream_MediaFile1idx","index","CREATE INDEX MediaFile_Stream_MediaFile1idx ON MediaFile_Stream_ (MediaFile1)"));
    res.push_back(Database::SchemaItem("MediaFile_Stream_Stream2idx","index","CREATE INDEX MediaFile_Stream_Stream2idx ON MediaFile_Stream_ (Stream2)"));
    res.push_back(Database::SchemaItem("MediaFile_Stream__all_idx","index","CREATE INDEX MediaFile_Stream__all_idx ON MediaFile_Stream_ (MediaFile1,Stream2)"));
    res.push_back(Database::SchemaItem("Job_MediaFile_JobInFileJob1idx","index","CREATE INDEX Job_MediaFile_JobInFileJob1idx ON Job_MediaFile_JobInFile (Job1)"));
    res.push_back(Database::SchemaItem("_75bd42a54265ee2791a61f8bfd916820","index","CREATE INDEX _75bd42a54265ee2791a61f8bfd916820 ON Job_MediaFile_JobInFile (MediaFile2)"));
    res.push_back(Database::SchemaItem("Job_MediaFile_JobInFile_all_idx","index","CREATE INDEX Job_MediaFile_JobInFile_all_idx ON Job_MediaFile_JobInFile (Job1,MediaFile2)"));
    res.push_back(Database::SchemaItem("Job_MediaFile_JobOutFileJob1idx","index","CREATE INDEX Job_MediaFile_JobOutFileJob1idx ON Job_MediaFile_JobOutFile (Job1)"));
    res.push_back(Database::SchemaItem("_25cc5418e8e8e4440a05457be79c6e6c","index","CREATE INDEX _25cc5418e8e8e4440a05457be79c6e6c ON Job_MediaFile_JobOutFile (MediaFile2)"));
    res.push_back(Database::SchemaItem("_d9d2d6c9fa1fb063c88ce1c8efda4f10","index","CREATE INDEX _d9d2d6c9fa1fb063c88ce1c8efda4f10 ON Job_MediaFile_JobOutFile (Job1,MediaFile2)"));
    res.push_back(Database::SchemaItem("_d42e07c973df5c1aa22424f96e07d102","index","CREATE INDEX _d42e07c973df5c1aa22424f96e07d102 ON Job_JobDetail_JobJobDetail (Job1)"));
    res.push_back(Database::SchemaItem("_f7ff1861fc4c003b6fc460836dea52b3","index","CREATE INDEX _f7ff1861fc4c003b6fc460836dea52b3 ON Job_JobDetail_JobJobDetail (JobDetail2)"));
    res.push_back(Database::SchemaItem("_cce4f09c9234ab0eb75047f7dd450a33","index","CREATE INDEX _cce4f09c9234ab0eb75047f7dd450a33 ON Job_JobDetail_JobJobDetail (Job1,JobDetail2)"));
    res.push_back(Database::SchemaItem("_9a6828d784e55ba8bb73831d4bf76738","index","CREATE INDEX _9a6828d784e55ba8bb73831d4bf76738 ON JobDetail_Stream_JobOutStream (JobDetail1)"));
    res.push_back(Database::SchemaItem("_47ba6e36c3117f932ca6ec94c3ba96fa","index","CREATE INDEX _47ba6e36c3117f932ca6ec94c3ba96fa ON JobDetail_Stream_JobOutStream (Stream2)"));
    res.push_back(Database::SchemaItem("_379b3c10ff7858af2b7df3132c35bae0","index","CREATE INDEX _379b3c10ff7858af2b7df3132c35bae0 ON JobDetail_Stream_JobOutStream (JobDetail1,Stream2)"));
    res.push_back(Database::SchemaItem("_b5fcb088a1d00510d53f08a2da9d354c","index","CREATE INDEX _b5fcb088a1d00510d53f08a2da9d354c ON JobDetail_Stream_JobInStream (JobDetail1)"));
    res.push_back(Database::SchemaItem("_3020cd32e6cb495b9b457666f03f8a53","index","CREATE INDEX _3020cd32e6cb495b9b457666f03f8a53 ON JobDetail_Stream_JobInStream (Stream2)"));
    res.push_back(Database::SchemaItem("_55609d0353fe020d7d52815110be116e","index","CREATE INDEX _55609d0353fe020d7d52815110be116e ON JobDetail_Stream_JobInStream (JobDetail1,Stream2)"));
    res.push_back(Database::SchemaItem("_7a62c43900ab70f3c419ad87a6111c3a","index","CREATE INDEX _7a62c43900ab70f3c419ad87a6111c3a ON _72915fab98e40e57ddd1495ecd15b95b (Profile1)"));
    res.push_back(Database::SchemaItem("_e4fc63ccedb1d89ed94759a0260215ab","index","CREATE INDEX _e4fc63ccedb1d89ed94759a0260215ab ON _72915fab98e40e57ddd1495ecd15b95b (Watchfolder2)"));
    res.push_back(Database::SchemaItem("_cbe16244cb3e6e81e642ee01cc56214b","index","CREATE INDEX _cbe16244cb3e6e81e642ee01cc56214b ON _72915fab98e40e57ddd1495ecd15b95b (Profile1,Watchfolder2)"));
    return res;
}
void HiveDb::initialize() {
    static bool initialized = false;
    if (initialized)
        return;
    initialized = true;
}
}
