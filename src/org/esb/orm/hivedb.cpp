#include "hivedb.hpp"
namespace orm {
using namespace litesql;
MediaFileMetaDataRelationMediaFile2MetaData::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : metaData(MediaFileMetaDataRelationMediaFile2MetaData::MetaData), mediaFile(MediaFileMetaDataRelationMediaFile2MetaData::MediaFile) {
    switch(rec.size()) {
    case 2:
        metaData = rec[1];
    case 1:
        mediaFile = rec[0];
    }
}
const std::string MediaFileMetaDataRelationMediaFile2MetaData::table__("_b861a1aecfe51793ff0c4d3ff4904353");
const litesql::FieldType MediaFileMetaDataRelationMediaFile2MetaData::MediaFile("MediaFile1","INTEGER",table__);
const litesql::FieldType MediaFileMetaDataRelationMediaFile2MetaData::MetaData("MetaData2","INTEGER",table__);
void MediaFileMetaDataRelationMediaFile2MetaData::link(const litesql::Database& db, const orm::MediaFile& o0, const orm::MetaData& o1) {
    Record values;
    Split fields;
    fields.push_back(MediaFile.name());
    values.push_back(o0.id);
    fields.push_back(MetaData.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void MediaFileMetaDataRelationMediaFile2MetaData::unlink(const litesql::Database& db, const orm::MediaFile& o0, const orm::MetaData& o1) {
    db.delete_(table__, (MediaFile == o0.id && MetaData == o1.id));
}
void MediaFileMetaDataRelationMediaFile2MetaData::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<MediaFileMetaDataRelationMediaFile2MetaData::Row> MediaFileMetaDataRelationMediaFile2MetaData::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(MediaFile.fullName());
    sel.result(MetaData.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<MediaFileMetaDataRelationMediaFile2MetaData::Row>(db, sel);
}
template <> litesql::DataSource<orm::MediaFile> MediaFileMetaDataRelationMediaFile2MetaData::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(MediaFile.fullName());
    sel.where(srcExpr);
    return DataSource<orm::MediaFile>(db, orm::MediaFile::Id.in(sel) && expr);
}
template <> litesql::DataSource<orm::MetaData> MediaFileMetaDataRelationMediaFile2MetaData::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(MetaData.fullName());
    sel.where(srcExpr);
    return DataSource<orm::MetaData>(db, orm::MetaData::Id.in(sel) && expr);
}
MediaFileProjectRelationProject2OutMediaFile::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : project(MediaFileProjectRelationProject2OutMediaFile::Project), mediaFile(MediaFileProjectRelationProject2OutMediaFile::MediaFile) {
    switch(rec.size()) {
    case 2:
        project = rec[1];
    case 1:
        mediaFile = rec[0];
    }
}
const std::string MediaFileProjectRelationProject2OutMediaFile::table__("_87d2faa689e591de47c1005beca3a5ee");
const litesql::FieldType MediaFileProjectRelationProject2OutMediaFile::MediaFile("MediaFile1","INTEGER",table__);
const litesql::FieldType MediaFileProjectRelationProject2OutMediaFile::Project("Project2","INTEGER",table__);
void MediaFileProjectRelationProject2OutMediaFile::link(const litesql::Database& db, const orm::MediaFile& o0, const orm::Project& o1) {
    Record values;
    Split fields;
    fields.push_back(MediaFile.name());
    values.push_back(o0.id);
    fields.push_back(Project.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void MediaFileProjectRelationProject2OutMediaFile::unlink(const litesql::Database& db, const orm::MediaFile& o0, const orm::Project& o1) {
    db.delete_(table__, (MediaFile == o0.id && Project == o1.id));
}
void MediaFileProjectRelationProject2OutMediaFile::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<MediaFileProjectRelationProject2OutMediaFile::Row> MediaFileProjectRelationProject2OutMediaFile::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(MediaFile.fullName());
    sel.result(Project.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<MediaFileProjectRelationProject2OutMediaFile::Row>(db, sel);
}
template <> litesql::DataSource<orm::MediaFile> MediaFileProjectRelationProject2OutMediaFile::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(MediaFile.fullName());
    sel.where(srcExpr);
    return DataSource<orm::MediaFile>(db, orm::MediaFile::Id.in(sel) && expr);
}
template <> litesql::DataSource<orm::Project> MediaFileProjectRelationProject2OutMediaFile::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Project.fullName());
    sel.where(srcExpr);
    return DataSource<orm::Project>(db, orm::Project::Id.in(sel) && expr);
}
MediaFileProjectRelationProject2InMediaFile::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : project(MediaFileProjectRelationProject2InMediaFile::Project), mediaFile(MediaFileProjectRelationProject2InMediaFile::MediaFile) {
    switch(rec.size()) {
    case 2:
        project = rec[1];
    case 1:
        mediaFile = rec[0];
    }
}
const std::string MediaFileProjectRelationProject2InMediaFile::table__("_b94f8c79597ffb2f54a53a65524ba4ab");
const litesql::FieldType MediaFileProjectRelationProject2InMediaFile::MediaFile("MediaFile1","INTEGER",table__);
const litesql::FieldType MediaFileProjectRelationProject2InMediaFile::Project("Project2","INTEGER",table__);
void MediaFileProjectRelationProject2InMediaFile::link(const litesql::Database& db, const orm::MediaFile& o0, const orm::Project& o1) {
    Record values;
    Split fields;
    fields.push_back(MediaFile.name());
    values.push_back(o0.id);
    fields.push_back(Project.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void MediaFileProjectRelationProject2InMediaFile::unlink(const litesql::Database& db, const orm::MediaFile& o0, const orm::Project& o1) {
    db.delete_(table__, (MediaFile == o0.id && Project == o1.id));
}
void MediaFileProjectRelationProject2InMediaFile::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<MediaFileProjectRelationProject2InMediaFile::Row> MediaFileProjectRelationProject2InMediaFile::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(MediaFile.fullName());
    sel.result(Project.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<MediaFileProjectRelationProject2InMediaFile::Row>(db, sel);
}
template <> litesql::DataSource<orm::MediaFile> MediaFileProjectRelationProject2InMediaFile::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(MediaFile.fullName());
    sel.where(srcExpr);
    return DataSource<orm::MediaFile>(db, orm::MediaFile::Id.in(sel) && expr);
}
template <> litesql::DataSource<orm::Project> MediaFileProjectRelationProject2InMediaFile::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Project.fullName());
    sel.where(srcExpr);
    return DataSource<orm::Project>(db, orm::Project::Id.in(sel) && expr);
}
JobProjectRelationProject2Job::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : project(JobProjectRelationProject2Job::Project), job(JobProjectRelationProject2Job::Job) {
    switch(rec.size()) {
    case 2:
        project = rec[1];
    case 1:
        job = rec[0];
    }
}
const std::string JobProjectRelationProject2Job::table__("Job_Project_Project2Job");
const litesql::FieldType JobProjectRelationProject2Job::Job("Job1","INTEGER",table__);
const litesql::FieldType JobProjectRelationProject2Job::Project("Project2","INTEGER",table__);
void JobProjectRelationProject2Job::link(const litesql::Database& db, const orm::Job& o0, const orm::Project& o1) {
    Record values;
    Split fields;
    fields.push_back(Job.name());
    values.push_back(o0.id);
    fields.push_back(Project.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void JobProjectRelationProject2Job::unlink(const litesql::Database& db, const orm::Job& o0, const orm::Project& o1) {
    db.delete_(table__, (Job == o0.id && Project == o1.id));
}
void JobProjectRelationProject2Job::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<JobProjectRelationProject2Job::Row> JobProjectRelationProject2Job::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Job.fullName());
    sel.result(Project.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<JobProjectRelationProject2Job::Row>(db, sel);
}
template <> litesql::DataSource<orm::Job> JobProjectRelationProject2Job::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Job.fullName());
    sel.where(srcExpr);
    return DataSource<orm::Job>(db, orm::Job::Id.in(sel) && expr);
}
template <> litesql::DataSource<orm::Project> JobProjectRelationProject2Job::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Project.fullName());
    sel.where(srcExpr);
    return DataSource<orm::Project>(db, orm::Project::Id.in(sel) && expr);
}
JobJobLogRelationJob2JobLog::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : jobLog(JobJobLogRelationJob2JobLog::JobLog), job(JobJobLogRelationJob2JobLog::Job) {
    switch(rec.size()) {
    case 2:
        jobLog = rec[1];
    case 1:
        job = rec[0];
    }
}
const std::string JobJobLogRelationJob2JobLog::table__("Job_JobLog_Job2JobLog");
const litesql::FieldType JobJobLogRelationJob2JobLog::Job("Job1","INTEGER",table__);
const litesql::FieldType JobJobLogRelationJob2JobLog::JobLog("JobLog2","INTEGER",table__);
void JobJobLogRelationJob2JobLog::link(const litesql::Database& db, const orm::Job& o0, const orm::JobLog& o1) {
    Record values;
    Split fields;
    fields.push_back(Job.name());
    values.push_back(o0.id);
    fields.push_back(JobLog.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void JobJobLogRelationJob2JobLog::unlink(const litesql::Database& db, const orm::Job& o0, const orm::JobLog& o1) {
    db.delete_(table__, (Job == o0.id && JobLog == o1.id));
}
void JobJobLogRelationJob2JobLog::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<JobJobLogRelationJob2JobLog::Row> JobJobLogRelationJob2JobLog::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Job.fullName());
    sel.result(JobLog.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<JobJobLogRelationJob2JobLog::Row>(db, sel);
}
template <> litesql::DataSource<orm::Job> JobJobLogRelationJob2JobLog::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Job.fullName());
    sel.where(srcExpr);
    return DataSource<orm::Job>(db, orm::Job::Id.in(sel) && expr);
}
template <> litesql::DataSource<orm::JobLog> JobJobLogRelationJob2JobLog::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(JobLog.fullName());
    sel.where(srcExpr);
    return DataSource<orm::JobLog>(db, orm::JobLog::Id.in(sel) && expr);
}
PresetProjectRelationProject2Preset::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : project(PresetProjectRelationProject2Preset::Project), preset(PresetProjectRelationProject2Preset::Preset) {
    switch(rec.size()) {
    case 2:
        project = rec[1];
    case 1:
        preset = rec[0];
    }
}
const std::string PresetProjectRelationProject2Preset::table__("Preset_Project_Project2Preset");
const litesql::FieldType PresetProjectRelationProject2Preset::Preset("Preset1","INTEGER",table__);
const litesql::FieldType PresetProjectRelationProject2Preset::Project("Project2","INTEGER",table__);
void PresetProjectRelationProject2Preset::link(const litesql::Database& db, const orm::Preset& o0, const orm::Project& o1) {
    Record values;
    Split fields;
    fields.push_back(Preset.name());
    values.push_back(o0.id);
    fields.push_back(Project.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void PresetProjectRelationProject2Preset::unlink(const litesql::Database& db, const orm::Preset& o0, const orm::Project& o1) {
    db.delete_(table__, (Preset == o0.id && Project == o1.id));
}
void PresetProjectRelationProject2Preset::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<PresetProjectRelationProject2Preset::Row> PresetProjectRelationProject2Preset::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Preset.fullName());
    sel.result(Project.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<PresetProjectRelationProject2Preset::Row>(db, sel);
}
template <> litesql::DataSource<orm::Preset> PresetProjectRelationProject2Preset::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Preset.fullName());
    sel.where(srcExpr);
    return DataSource<orm::Preset>(db, orm::Preset::Id.in(sel) && expr);
}
template <> litesql::DataSource<orm::Project> PresetProjectRelationProject2Preset::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Project.fullName());
    sel.where(srcExpr);
    return DataSource<orm::Project>(db, orm::Project::Id.in(sel) && expr);
}
CodecPresetPresetRelationPreset2CodecPreset::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : preset(CodecPresetPresetRelationPreset2CodecPreset::Preset), codecPreset(CodecPresetPresetRelationPreset2CodecPreset::CodecPreset) {
    switch(rec.size()) {
    case 2:
        preset = rec[1];
    case 1:
        codecPreset = rec[0];
    }
}
const std::string CodecPresetPresetRelationPreset2CodecPreset::table__("_fde0f636acb9734e36447df73bd4c50c");
const litesql::FieldType CodecPresetPresetRelationPreset2CodecPreset::CodecPreset("CodecPreset1","INTEGER",table__);
const litesql::FieldType CodecPresetPresetRelationPreset2CodecPreset::Preset("Preset2","INTEGER",table__);
void CodecPresetPresetRelationPreset2CodecPreset::link(const litesql::Database& db, const orm::CodecPreset& o0, const orm::Preset& o1) {
    Record values;
    Split fields;
    fields.push_back(CodecPreset.name());
    values.push_back(o0.id);
    fields.push_back(Preset.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void CodecPresetPresetRelationPreset2CodecPreset::unlink(const litesql::Database& db, const orm::CodecPreset& o0, const orm::Preset& o1) {
    db.delete_(table__, (CodecPreset == o0.id && Preset == o1.id));
}
void CodecPresetPresetRelationPreset2CodecPreset::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<CodecPresetPresetRelationPreset2CodecPreset::Row> CodecPresetPresetRelationPreset2CodecPreset::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(CodecPreset.fullName());
    sel.result(Preset.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<CodecPresetPresetRelationPreset2CodecPreset::Row>(db, sel);
}
template <> litesql::DataSource<orm::CodecPreset> CodecPresetPresetRelationPreset2CodecPreset::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(CodecPreset.fullName());
    sel.where(srcExpr);
    return DataSource<orm::CodecPreset>(db, orm::CodecPreset::Id.in(sel) && expr);
}
template <> litesql::DataSource<orm::Preset> CodecPresetPresetRelationPreset2CodecPreset::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Preset.fullName());
    sel.where(srcExpr);
    return DataSource<orm::Preset>(db, orm::Preset::Id.in(sel) && expr);
}
FilterPresetRelationPreset2Filter::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : preset(FilterPresetRelationPreset2Filter::Preset), filter(FilterPresetRelationPreset2Filter::Filter) {
    switch(rec.size()) {
    case 2:
        preset = rec[1];
    case 1:
        filter = rec[0];
    }
}
const std::string FilterPresetRelationPreset2Filter::table__("Filter_Preset_Preset2Filter");
const litesql::FieldType FilterPresetRelationPreset2Filter::Filter("Filter1","INTEGER",table__);
const litesql::FieldType FilterPresetRelationPreset2Filter::Preset("Preset2","INTEGER",table__);
void FilterPresetRelationPreset2Filter::link(const litesql::Database& db, const orm::Filter& o0, const orm::Preset& o1) {
    Record values;
    Split fields;
    fields.push_back(Filter.name());
    values.push_back(o0.id);
    fields.push_back(Preset.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void FilterPresetRelationPreset2Filter::unlink(const litesql::Database& db, const orm::Filter& o0, const orm::Preset& o1) {
    db.delete_(table__, (Filter == o0.id && Preset == o1.id));
}
void FilterPresetRelationPreset2Filter::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<FilterPresetRelationPreset2Filter::Row> FilterPresetRelationPreset2Filter::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Filter.fullName());
    sel.result(Preset.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<FilterPresetRelationPreset2Filter::Row>(db, sel);
}
template <> litesql::DataSource<orm::Filter> FilterPresetRelationPreset2Filter::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Filter.fullName());
    sel.where(srcExpr);
    return DataSource<orm::Filter>(db, orm::Filter::Id.in(sel) && expr);
}
template <> litesql::DataSource<orm::Preset> FilterPresetRelationPreset2Filter::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Preset.fullName());
    sel.where(srcExpr);
    return DataSource<orm::Preset>(db, orm::Preset::Id.in(sel) && expr);
}
FilterFilterParameterRelationFilter2FilterParameter::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : filterParameter(FilterFilterParameterRelationFilter2FilterParameter::FilterParameter), filter(FilterFilterParameterRelationFilter2FilterParameter::Filter) {
    switch(rec.size()) {
    case 2:
        filterParameter = rec[1];
    case 1:
        filter = rec[0];
    }
}
const std::string FilterFilterParameterRelationFilter2FilterParameter::table__("_0122558569afdaace79061607b81cf8c");
const litesql::FieldType FilterFilterParameterRelationFilter2FilterParameter::Filter("Filter1","INTEGER",table__);
const litesql::FieldType FilterFilterParameterRelationFilter2FilterParameter::FilterParameter("FilterParameter2","INTEGER",table__);
void FilterFilterParameterRelationFilter2FilterParameter::link(const litesql::Database& db, const orm::Filter& o0, const orm::FilterParameter& o1) {
    Record values;
    Split fields;
    fields.push_back(Filter.name());
    values.push_back(o0.id);
    fields.push_back(FilterParameter.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void FilterFilterParameterRelationFilter2FilterParameter::unlink(const litesql::Database& db, const orm::Filter& o0, const orm::FilterParameter& o1) {
    db.delete_(table__, (Filter == o0.id && FilterParameter == o1.id));
}
void FilterFilterParameterRelationFilter2FilterParameter::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<FilterFilterParameterRelationFilter2FilterParameter::Row> FilterFilterParameterRelationFilter2FilterParameter::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Filter.fullName());
    sel.result(FilterParameter.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<FilterFilterParameterRelationFilter2FilterParameter::Row>(db, sel);
}
template <> litesql::DataSource<orm::Filter> FilterFilterParameterRelationFilter2FilterParameter::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Filter.fullName());
    sel.where(srcExpr);
    return DataSource<orm::Filter>(db, orm::Filter::Id.in(sel) && expr);
}
template <> litesql::DataSource<orm::FilterParameter> FilterFilterParameterRelationFilter2FilterParameter::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(FilterParameter.fullName());
    sel.where(srcExpr);
    return DataSource<orm::FilterParameter>(db, orm::FilterParameter::Id.in(sel) && expr);
}
FormatPresetRelationPreset2Format::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : preset(FormatPresetRelationPreset2Format::Preset), format(FormatPresetRelationPreset2Format::Format) {
    switch(rec.size()) {
    case 2:
        preset = rec[1];
    case 1:
        format = rec[0];
    }
}
const std::string FormatPresetRelationPreset2Format::table__("Format_Preset_Preset2Format");
const litesql::FieldType FormatPresetRelationPreset2Format::Format("Format1","INTEGER",table__);
const litesql::FieldType FormatPresetRelationPreset2Format::Preset("Preset2","INTEGER",table__);
void FormatPresetRelationPreset2Format::link(const litesql::Database& db, const orm::Format& o0, const orm::Preset& o1) {
    Record values;
    Split fields;
    fields.push_back(Format.name());
    values.push_back(o0.id);
    fields.push_back(Preset.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void FormatPresetRelationPreset2Format::unlink(const litesql::Database& db, const orm::Format& o0, const orm::Preset& o1) {
    db.delete_(table__, (Format == o0.id && Preset == o1.id));
}
void FormatPresetRelationPreset2Format::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<FormatPresetRelationPreset2Format::Row> FormatPresetRelationPreset2Format::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Format.fullName());
    sel.result(Preset.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<FormatPresetRelationPreset2Format::Row>(db, sel);
}
template <> litesql::DataSource<orm::Format> FormatPresetRelationPreset2Format::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Format.fullName());
    sel.where(srcExpr);
    return DataSource<orm::Format>(db, orm::Format::Id.in(sel) && expr);
}
template <> litesql::DataSource<orm::Preset> FormatPresetRelationPreset2Format::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Preset.fullName());
    sel.where(srcExpr);
    return DataSource<orm::Preset>(db, orm::Preset::Id.in(sel) && expr);
}
FormatFormatParameterRelationFormat2FormatParameter::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : formatParameter(FormatFormatParameterRelationFormat2FormatParameter::FormatParameter), format(FormatFormatParameterRelationFormat2FormatParameter::Format) {
    switch(rec.size()) {
    case 2:
        formatParameter = rec[1];
    case 1:
        format = rec[0];
    }
}
const std::string FormatFormatParameterRelationFormat2FormatParameter::table__("_0fd44753f20129838ffe2f1c0dd8acc7");
const litesql::FieldType FormatFormatParameterRelationFormat2FormatParameter::Format("Format1","INTEGER",table__);
const litesql::FieldType FormatFormatParameterRelationFormat2FormatParameter::FormatParameter("FormatParameter2","INTEGER",table__);
void FormatFormatParameterRelationFormat2FormatParameter::link(const litesql::Database& db, const orm::Format& o0, const orm::FormatParameter& o1) {
    Record values;
    Split fields;
    fields.push_back(Format.name());
    values.push_back(o0.id);
    fields.push_back(FormatParameter.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void FormatFormatParameterRelationFormat2FormatParameter::unlink(const litesql::Database& db, const orm::Format& o0, const orm::FormatParameter& o1) {
    db.delete_(table__, (Format == o0.id && FormatParameter == o1.id));
}
void FormatFormatParameterRelationFormat2FormatParameter::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<FormatFormatParameterRelationFormat2FormatParameter::Row> FormatFormatParameterRelationFormat2FormatParameter::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Format.fullName());
    sel.result(FormatParameter.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<FormatFormatParameterRelationFormat2FormatParameter::Row>(db, sel);
}
template <> litesql::DataSource<orm::Format> FormatFormatParameterRelationFormat2FormatParameter::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Format.fullName());
    sel.where(srcExpr);
    return DataSource<orm::Format>(db, orm::Format::Id.in(sel) && expr);
}
template <> litesql::DataSource<orm::FormatParameter> FormatFormatParameterRelationFormat2FormatParameter::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(FormatParameter.fullName());
    sel.where(srcExpr);
    return DataSource<orm::FormatParameter>(db, orm::FormatParameter::Id.in(sel) && expr);
}
MediaFileStreamRelationMediaFile2Stream::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : stream(MediaFileStreamRelationMediaFile2Stream::Stream), mediaFile(MediaFileStreamRelationMediaFile2Stream::MediaFile) {
    switch(rec.size()) {
    case 2:
        stream = rec[1];
    case 1:
        mediaFile = rec[0];
    }
}
const std::string MediaFileStreamRelationMediaFile2Stream::table__("_59ad23f924ca1b82be6749fba38a4d77");
const litesql::FieldType MediaFileStreamRelationMediaFile2Stream::MediaFile("MediaFile1","INTEGER",table__);
const litesql::FieldType MediaFileStreamRelationMediaFile2Stream::Stream("Stream2","INTEGER",table__);
void MediaFileStreamRelationMediaFile2Stream::link(const litesql::Database& db, const orm::MediaFile& o0, const orm::Stream& o1) {
    Record values;
    Split fields;
    fields.push_back(MediaFile.name());
    values.push_back(o0.id);
    fields.push_back(Stream.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void MediaFileStreamRelationMediaFile2Stream::unlink(const litesql::Database& db, const orm::MediaFile& o0, const orm::Stream& o1) {
    db.delete_(table__, (MediaFile == o0.id && Stream == o1.id));
}
void MediaFileStreamRelationMediaFile2Stream::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<MediaFileStreamRelationMediaFile2Stream::Row> MediaFileStreamRelationMediaFile2Stream::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(MediaFile.fullName());
    sel.result(Stream.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<MediaFileStreamRelationMediaFile2Stream::Row>(db, sel);
}
template <> litesql::DataSource<orm::MediaFile> MediaFileStreamRelationMediaFile2Stream::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(MediaFile.fullName());
    sel.where(srcExpr);
    return DataSource<orm::MediaFile>(db, orm::MediaFile::Id.in(sel) && expr);
}
template <> litesql::DataSource<orm::Stream> MediaFileStreamRelationMediaFile2Stream::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Stream.fullName());
    sel.where(srcExpr);
    return DataSource<orm::Stream>(db, orm::Stream::Id.in(sel) && expr);
}
FormatStreamRelationStream2Format::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : stream(FormatStreamRelationStream2Format::Stream), format(FormatStreamRelationStream2Format::Format) {
    switch(rec.size()) {
    case 2:
        stream = rec[1];
    case 1:
        format = rec[0];
    }
}
const std::string FormatStreamRelationStream2Format::table__("Format_Stream_Stream2Format");
const litesql::FieldType FormatStreamRelationStream2Format::Format("Format1","INTEGER",table__);
const litesql::FieldType FormatStreamRelationStream2Format::Stream("Stream2","INTEGER",table__);
void FormatStreamRelationStream2Format::link(const litesql::Database& db, const orm::Format& o0, const orm::Stream& o1) {
    Record values;
    Split fields;
    fields.push_back(Format.name());
    values.push_back(o0.id);
    fields.push_back(Stream.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void FormatStreamRelationStream2Format::unlink(const litesql::Database& db, const orm::Format& o0, const orm::Stream& o1) {
    db.delete_(table__, (Format == o0.id && Stream == o1.id));
}
void FormatStreamRelationStream2Format::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<FormatStreamRelationStream2Format::Row> FormatStreamRelationStream2Format::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Format.fullName());
    sel.result(Stream.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<FormatStreamRelationStream2Format::Row>(db, sel);
}
template <> litesql::DataSource<orm::Format> FormatStreamRelationStream2Format::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Format.fullName());
    sel.where(srcExpr);
    return DataSource<orm::Format>(db, orm::Format::Id.in(sel) && expr);
}
template <> litesql::DataSource<orm::Stream> FormatStreamRelationStream2Format::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Stream.fullName());
    sel.where(srcExpr);
    return DataSource<orm::Stream>(db, orm::Stream::Id.in(sel) && expr);
}
ProcessUnitStreamRelationStream2ProcessUnit::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : stream(ProcessUnitStreamRelationStream2ProcessUnit::Stream), processUnit(ProcessUnitStreamRelationStream2ProcessUnit::ProcessUnit) {
    switch(rec.size()) {
    case 2:
        stream = rec[1];
    case 1:
        processUnit = rec[0];
    }
}
const std::string ProcessUnitStreamRelationStream2ProcessUnit::table__("_0bd390801550415f76a329f9b7842914");
const litesql::FieldType ProcessUnitStreamRelationStream2ProcessUnit::ProcessUnit("ProcessUnit1","INTEGER",table__);
const litesql::FieldType ProcessUnitStreamRelationStream2ProcessUnit::Stream("Stream2","INTEGER",table__);
void ProcessUnitStreamRelationStream2ProcessUnit::link(const litesql::Database& db, const orm::ProcessUnit& o0, const orm::Stream& o1) {
    Record values;
    Split fields;
    fields.push_back(ProcessUnit.name());
    values.push_back(o0.id);
    fields.push_back(Stream.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void ProcessUnitStreamRelationStream2ProcessUnit::unlink(const litesql::Database& db, const orm::ProcessUnit& o0, const orm::Stream& o1) {
    db.delete_(table__, (ProcessUnit == o0.id && Stream == o1.id));
}
void ProcessUnitStreamRelationStream2ProcessUnit::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<ProcessUnitStreamRelationStream2ProcessUnit::Row> ProcessUnitStreamRelationStream2ProcessUnit::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(ProcessUnit.fullName());
    sel.result(Stream.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<ProcessUnitStreamRelationStream2ProcessUnit::Row>(db, sel);
}
template <> litesql::DataSource<orm::ProcessUnit> ProcessUnitStreamRelationStream2ProcessUnit::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(ProcessUnit.fullName());
    sel.where(srcExpr);
    return DataSource<orm::ProcessUnit>(db, orm::ProcessUnit::Id.in(sel) && expr);
}
template <> litesql::DataSource<orm::Stream> ProcessUnitStreamRelationStream2ProcessUnit::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Stream.fullName());
    sel.where(srcExpr);
    return DataSource<orm::Stream>(db, orm::Stream::Id.in(sel) && expr);
}
const litesql::FieldType MediaFile::Own::Id("id_","INTEGER","MediaFile_");
MediaFile::MetadataHandle::MetadataHandle(const MediaFile& owner)
         : litesql::RelationHandle<MediaFile>(owner) {
}
void MediaFile::MetadataHandle::link(const MetaData& o0) {
    MediaFileMetaDataRelationMediaFile2MetaData::link(owner->getDatabase(), *owner, o0);
}
void MediaFile::MetadataHandle::unlink(const MetaData& o0) {
    MediaFileMetaDataRelationMediaFile2MetaData::unlink(owner->getDatabase(), *owner, o0);
}
void MediaFile::MetadataHandle::del(const litesql::Expr& expr) {
    MediaFileMetaDataRelationMediaFile2MetaData::del(owner->getDatabase(), expr && MediaFileMetaDataRelationMediaFile2MetaData::MediaFile == owner->id);
}
litesql::DataSource<MetaData> MediaFile::MetadataHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return MediaFileMetaDataRelationMediaFile2MetaData::get<MetaData>(owner->getDatabase(), expr, (MediaFileMetaDataRelationMediaFile2MetaData::MediaFile == owner->id) && srcExpr);
}
litesql::DataSource<MediaFileMetaDataRelationMediaFile2MetaData::Row> MediaFile::MetadataHandle::getRows(const litesql::Expr& expr) {
    return MediaFileMetaDataRelationMediaFile2MetaData::getRows(owner->getDatabase(), expr && (MediaFileMetaDataRelationMediaFile2MetaData::MediaFile == owner->id));
}
MediaFile::ProjectoutfileHandle::ProjectoutfileHandle(const MediaFile& owner)
         : litesql::RelationHandle<MediaFile>(owner) {
}
void MediaFile::ProjectoutfileHandle::link(const Project& o0) {
    MediaFileProjectRelationProject2OutMediaFile::link(owner->getDatabase(), *owner, o0);
}
void MediaFile::ProjectoutfileHandle::unlink(const Project& o0) {
    MediaFileProjectRelationProject2OutMediaFile::unlink(owner->getDatabase(), *owner, o0);
}
void MediaFile::ProjectoutfileHandle::del(const litesql::Expr& expr) {
    MediaFileProjectRelationProject2OutMediaFile::del(owner->getDatabase(), expr && MediaFileProjectRelationProject2OutMediaFile::MediaFile == owner->id);
}
litesql::DataSource<Project> MediaFile::ProjectoutfileHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return MediaFileProjectRelationProject2OutMediaFile::get<Project>(owner->getDatabase(), expr, (MediaFileProjectRelationProject2OutMediaFile::MediaFile == owner->id) && srcExpr);
}
litesql::DataSource<MediaFileProjectRelationProject2OutMediaFile::Row> MediaFile::ProjectoutfileHandle::getRows(const litesql::Expr& expr) {
    return MediaFileProjectRelationProject2OutMediaFile::getRows(owner->getDatabase(), expr && (MediaFileProjectRelationProject2OutMediaFile::MediaFile == owner->id));
}
MediaFile::ProjectinfileHandle::ProjectinfileHandle(const MediaFile& owner)
         : litesql::RelationHandle<MediaFile>(owner) {
}
void MediaFile::ProjectinfileHandle::link(const Project& o0) {
    MediaFileProjectRelationProject2InMediaFile::link(owner->getDatabase(), *owner, o0);
}
void MediaFile::ProjectinfileHandle::unlink(const Project& o0) {
    MediaFileProjectRelationProject2InMediaFile::unlink(owner->getDatabase(), *owner, o0);
}
void MediaFile::ProjectinfileHandle::del(const litesql::Expr& expr) {
    MediaFileProjectRelationProject2InMediaFile::del(owner->getDatabase(), expr && MediaFileProjectRelationProject2InMediaFile::MediaFile == owner->id);
}
litesql::DataSource<Project> MediaFile::ProjectinfileHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return MediaFileProjectRelationProject2InMediaFile::get<Project>(owner->getDatabase(), expr, (MediaFileProjectRelationProject2InMediaFile::MediaFile == owner->id) && srcExpr);
}
litesql::DataSource<MediaFileProjectRelationProject2InMediaFile::Row> MediaFile::ProjectinfileHandle::getRows(const litesql::Expr& expr) {
    return MediaFileProjectRelationProject2InMediaFile::getRows(owner->getDatabase(), expr && (MediaFileProjectRelationProject2InMediaFile::MediaFile == owner->id));
}
MediaFile::StreamsHandle::StreamsHandle(const MediaFile& owner)
         : litesql::RelationHandle<MediaFile>(owner) {
}
void MediaFile::StreamsHandle::link(const Stream& o0) {
    MediaFileStreamRelationMediaFile2Stream::link(owner->getDatabase(), *owner, o0);
}
void MediaFile::StreamsHandle::unlink(const Stream& o0) {
    MediaFileStreamRelationMediaFile2Stream::unlink(owner->getDatabase(), *owner, o0);
}
void MediaFile::StreamsHandle::del(const litesql::Expr& expr) {
    MediaFileStreamRelationMediaFile2Stream::del(owner->getDatabase(), expr && MediaFileStreamRelationMediaFile2Stream::MediaFile == owner->id);
}
litesql::DataSource<Stream> MediaFile::StreamsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return MediaFileStreamRelationMediaFile2Stream::get<Stream>(owner->getDatabase(), expr, (MediaFileStreamRelationMediaFile2Stream::MediaFile == owner->id) && srcExpr);
}
litesql::DataSource<MediaFileStreamRelationMediaFile2Stream::Row> MediaFile::StreamsHandle::getRows(const litesql::Expr& expr) {
    return MediaFileStreamRelationMediaFile2Stream::getRows(owner->getDatabase(), expr && (MediaFileStreamRelationMediaFile2Stream::MediaFile == owner->id));
}
const std::string MediaFile::type__("MediaFile");
const std::string MediaFile::table__("MediaFile_");
const std::string MediaFile::sequence__("MediaFile_seq");
const litesql::FieldType MediaFile::Id("id_","INTEGER",table__);
const litesql::FieldType MediaFile::Type("type_","TEXT",table__);
const litesql::FieldType MediaFile::Filepath("filepath_","TEXT",table__);
const litesql::FieldType MediaFile::Filename("filename_","TEXT",table__);
const litesql::FieldType MediaFile::Filesize("filesize_","DOUBLE",table__);
const litesql::FieldType MediaFile::Streamcount("streamcount_","INTEGER",table__);
const litesql::FieldType MediaFile::Containertype("containertype_","TEXT",table__);
const litesql::FieldType MediaFile::Duration("duration_","DOUBLE",table__);
const litesql::FieldType MediaFile::Starttime("starttime_","DOUBLE",table__);
const litesql::FieldType MediaFile::Bitrate("bitrate_","INTEGER",table__);
const litesql::FieldType MediaFile::Filetype("filetype_","INTEGER",table__);
const litesql::FieldType MediaFile::Parent("parent_","INTEGER",table__);
const litesql::FieldType MediaFile::Created("created_","INTEGER",table__);
void MediaFile::defaults() {
    id = 0;
    filesize = 0.0;
    streamcount = 0;
    duration = 0.0;
    starttime = 0.0;
    bitrate = 0;
    filetype = 0;
    parent = 0;
    created = 0;
}
MediaFile::MediaFile(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), filepath(Filepath), filename(Filename), filesize(Filesize), streamcount(Streamcount), containertype(Containertype), duration(Duration), starttime(Starttime), bitrate(Bitrate), filetype(Filetype), parent(Parent), created(Created) {
    defaults();
}
MediaFile::MediaFile(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), filepath(Filepath), filename(Filename), filesize(Filesize), streamcount(Streamcount), containertype(Containertype), duration(Duration), starttime(Starttime), bitrate(Bitrate), filetype(Filetype), parent(Parent), created(Created) {
    defaults();
    size_t size = (rec.size() > 13) ? 13 : rec.size();
    switch(size) {
    case 13: created = convert<const std::string&, litesql::DateTime>(rec[12]);
        created.setModified(false);
    case 12: parent = convert<const std::string&, int>(rec[11]);
        parent.setModified(false);
    case 11: filetype = convert<const std::string&, int>(rec[10]);
        filetype.setModified(false);
    case 10: bitrate = convert<const std::string&, int>(rec[9]);
        bitrate.setModified(false);
    case 9: starttime = convert<const std::string&, double>(rec[8]);
        starttime.setModified(false);
    case 8: duration = convert<const std::string&, double>(rec[7]);
        duration.setModified(false);
    case 7: containertype = convert<const std::string&, std::string>(rec[6]);
        containertype.setModified(false);
    case 6: streamcount = convert<const std::string&, int>(rec[5]);
        streamcount.setModified(false);
    case 5: filesize = convert<const std::string&, double>(rec[4]);
        filesize.setModified(false);
    case 4: filename = convert<const std::string&, std::string>(rec[3]);
        filename.setModified(false);
    case 3: filepath = convert<const std::string&, std::string>(rec[2]);
        filepath.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
MediaFile::MediaFile(const MediaFile& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), filepath(obj.filepath), filename(obj.filename), filesize(obj.filesize), streamcount(obj.streamcount), containertype(obj.containertype), duration(obj.duration), starttime(obj.starttime), bitrate(obj.bitrate), filetype(obj.filetype), parent(obj.parent), created(obj.created) {
}
const MediaFile& MediaFile::operator=(const MediaFile& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        filepath = obj.filepath;
        filename = obj.filename;
        filesize = obj.filesize;
        streamcount = obj.streamcount;
        containertype = obj.containertype;
        duration = obj.duration;
        starttime = obj.starttime;
        bitrate = obj.bitrate;
        filetype = obj.filetype;
        parent = obj.parent;
        created = obj.created;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
MediaFile::MetadataHandle MediaFile::metadata() {
    return MediaFile::MetadataHandle(*this);
}
MediaFile::ProjectoutfileHandle MediaFile::projectoutfile() {
    return MediaFile::ProjectoutfileHandle(*this);
}
MediaFile::ProjectinfileHandle MediaFile::projectinfile() {
    return MediaFile::ProjectinfileHandle(*this);
}
MediaFile::StreamsHandle MediaFile::streams() {
    return MediaFile::StreamsHandle(*this);
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
    fields.push_back(filepath.name());
    values.push_back(filepath);
    filepath.setModified(false);
    fields.push_back(filename.name());
    values.push_back(filename);
    filename.setModified(false);
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
    fields.push_back(starttime.name());
    values.push_back(starttime);
    starttime.setModified(false);
    fields.push_back(bitrate.name());
    values.push_back(bitrate);
    bitrate.setModified(false);
    fields.push_back(filetype.name());
    values.push_back(filetype);
    filetype.setModified(false);
    fields.push_back(parent.name());
    values.push_back(parent);
    parent.setModified(false);
    fields.push_back(created.name());
    values.push_back(created);
    created.setModified(false);
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
    updateField(updates, table__, filepath);
    updateField(updates, table__, filename);
    updateField(updates, table__, filesize);
    updateField(updates, table__, streamcount);
    updateField(updates, table__, containertype);
    updateField(updates, table__, duration);
    updateField(updates, table__, starttime);
    updateField(updates, table__, bitrate);
    updateField(updates, table__, filetype);
    updateField(updates, table__, parent);
    updateField(updates, table__, created);
}
void MediaFile::addIDUpdates(Updates& updates) {
}
void MediaFile::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Filepath);
    ftypes.push_back(Filename);
    ftypes.push_back(Filesize);
    ftypes.push_back(Streamcount);
    ftypes.push_back(Containertype);
    ftypes.push_back(Duration);
    ftypes.push_back(Starttime);
    ftypes.push_back(Bitrate);
    ftypes.push_back(Filetype);
    ftypes.push_back(Parent);
    ftypes.push_back(Created);
}
void MediaFile::delRecord() {
    deleteFromTable(table__, id);
}
void MediaFile::delRelations() {
    MediaFileMetaDataRelationMediaFile2MetaData::del(*db, (MediaFileMetaDataRelationMediaFile2MetaData::MediaFile == id));
    MediaFileProjectRelationProject2OutMediaFile::del(*db, (MediaFileProjectRelationProject2OutMediaFile::MediaFile == id));
    MediaFileProjectRelationProject2InMediaFile::del(*db, (MediaFileProjectRelationProject2InMediaFile::MediaFile == id));
    MediaFileStreamRelationMediaFile2Stream::del(*db, (MediaFileStreamRelationMediaFile2Stream::MediaFile == id));
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
    np->filepath = filepath;
    np->filename = filename;
    np->filesize = filesize;
    np->streamcount = streamcount;
    np->containertype = containertype;
    np->duration = duration;
    np->starttime = starttime;
    np->bitrate = bitrate;
    np->filetype = filetype;
    np->parent = parent;
    np->created = created;
    np->inDatabase = inDatabase;
    return auto_ptr<MediaFile>(np);
}
std::ostream & operator<<(std::ostream& os, MediaFile o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.filepath.name() << " = " << o.filepath << std::endl;
    os << o.filename.name() << " = " << o.filename << std::endl;
    os << o.filesize.name() << " = " << o.filesize << std::endl;
    os << o.streamcount.name() << " = " << o.streamcount << std::endl;
    os << o.containertype.name() << " = " << o.containertype << std::endl;
    os << o.duration.name() << " = " << o.duration << std::endl;
    os << o.starttime.name() << " = " << o.starttime << std::endl;
    os << o.bitrate.name() << " = " << o.bitrate << std::endl;
    os << o.filetype.name() << " = " << o.filetype << std::endl;
    os << o.parent.name() << " = " << o.parent << std::endl;
    os << o.created.name() << " = " << o.created << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType MetaData::Own::Id("id_","INTEGER","MetaData_");
MetaData::MediafileHandle::MediafileHandle(const MetaData& owner)
         : litesql::RelationHandle<MetaData>(owner) {
}
void MetaData::MediafileHandle::link(const MediaFile& o0) {
    MediaFileMetaDataRelationMediaFile2MetaData::link(owner->getDatabase(), o0, *owner);
}
void MetaData::MediafileHandle::unlink(const MediaFile& o0) {
    MediaFileMetaDataRelationMediaFile2MetaData::unlink(owner->getDatabase(), o0, *owner);
}
void MetaData::MediafileHandle::del(const litesql::Expr& expr) {
    MediaFileMetaDataRelationMediaFile2MetaData::del(owner->getDatabase(), expr && MediaFileMetaDataRelationMediaFile2MetaData::MetaData == owner->id);
}
litesql::DataSource<MediaFile> MetaData::MediafileHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return MediaFileMetaDataRelationMediaFile2MetaData::get<MediaFile>(owner->getDatabase(), expr, (MediaFileMetaDataRelationMediaFile2MetaData::MetaData == owner->id) && srcExpr);
}
litesql::DataSource<MediaFileMetaDataRelationMediaFile2MetaData::Row> MetaData::MediafileHandle::getRows(const litesql::Expr& expr) {
    return MediaFileMetaDataRelationMediaFile2MetaData::getRows(owner->getDatabase(), expr && (MediaFileMetaDataRelationMediaFile2MetaData::MetaData == owner->id));
}
const std::string MetaData::type__("MetaData");
const std::string MetaData::table__("MetaData_");
const std::string MetaData::sequence__("MetaData_seq");
const litesql::FieldType MetaData::Id("id_","INTEGER",table__);
const litesql::FieldType MetaData::Type("type_","TEXT",table__);
const litesql::FieldType MetaData::Metakey("metakey_","TEXT",table__);
const litesql::FieldType MetaData::Metavalue("metavalue_","TEXT",table__);
void MetaData::defaults() {
    id = 0;
}
MetaData::MetaData(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), metakey(Metakey), metavalue(Metavalue) {
    defaults();
}
MetaData::MetaData(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), metakey(Metakey), metavalue(Metavalue) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: metavalue = convert<const std::string&, std::string>(rec[3]);
        metavalue.setModified(false);
    case 3: metakey = convert<const std::string&, std::string>(rec[2]);
        metakey.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
MetaData::MetaData(const MetaData& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), metakey(obj.metakey), metavalue(obj.metavalue) {
}
const MetaData& MetaData::operator=(const MetaData& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        metakey = obj.metakey;
        metavalue = obj.metavalue;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
MetaData::MediafileHandle MetaData::mediafile() {
    return MetaData::MediafileHandle(*this);
}
std::string MetaData::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(metakey.name());
    values.push_back(metakey);
    metakey.setModified(false);
    fields.push_back(metavalue.name());
    values.push_back(metavalue);
    metavalue.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void MetaData::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void MetaData::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, metakey);
    updateField(updates, table__, metavalue);
}
void MetaData::addIDUpdates(Updates& updates) {
}
void MetaData::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Metakey);
    ftypes.push_back(Metavalue);
}
void MetaData::delRecord() {
    deleteFromTable(table__, id);
}
void MetaData::delRelations() {
    MediaFileMetaDataRelationMediaFile2MetaData::del(*db, (MediaFileMetaDataRelationMediaFile2MetaData::MetaData == id));
}
void MetaData::update() {
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
void MetaData::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<MetaData> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool MetaData::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<MetaData> MetaData::upcast() {
    return auto_ptr<MetaData>(new MetaData(*this));
}
std::auto_ptr<MetaData> MetaData::upcastCopy() {
    MetaData* np = new MetaData(*this);
    np->id = id;
    np->type = type;
    np->metakey = metakey;
    np->metavalue = metavalue;
    np->inDatabase = inDatabase;
    return auto_ptr<MetaData>(np);
}
std::ostream & operator<<(std::ostream& os, MetaData o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.metakey.name() << " = " << o.metakey << std::endl;
    os << o.metavalue.name() << " = " << o.metavalue << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Project::Own::Id("id_","INTEGER","Project_");
Project::OutmediafilesHandle::OutmediafilesHandle(const Project& owner)
         : litesql::RelationHandle<Project>(owner) {
}
void Project::OutmediafilesHandle::link(const MediaFile& o0) {
    MediaFileProjectRelationProject2OutMediaFile::link(owner->getDatabase(), o0, *owner);
}
void Project::OutmediafilesHandle::unlink(const MediaFile& o0) {
    MediaFileProjectRelationProject2OutMediaFile::unlink(owner->getDatabase(), o0, *owner);
}
void Project::OutmediafilesHandle::del(const litesql::Expr& expr) {
    MediaFileProjectRelationProject2OutMediaFile::del(owner->getDatabase(), expr && MediaFileProjectRelationProject2OutMediaFile::Project == owner->id);
}
litesql::DataSource<MediaFile> Project::OutmediafilesHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return MediaFileProjectRelationProject2OutMediaFile::get<MediaFile>(owner->getDatabase(), expr, (MediaFileProjectRelationProject2OutMediaFile::Project == owner->id) && srcExpr);
}
litesql::DataSource<MediaFileProjectRelationProject2OutMediaFile::Row> Project::OutmediafilesHandle::getRows(const litesql::Expr& expr) {
    return MediaFileProjectRelationProject2OutMediaFile::getRows(owner->getDatabase(), expr && (MediaFileProjectRelationProject2OutMediaFile::Project == owner->id));
}
Project::InmediafilesHandle::InmediafilesHandle(const Project& owner)
         : litesql::RelationHandle<Project>(owner) {
}
void Project::InmediafilesHandle::link(const MediaFile& o0) {
    MediaFileProjectRelationProject2InMediaFile::link(owner->getDatabase(), o0, *owner);
}
void Project::InmediafilesHandle::unlink(const MediaFile& o0) {
    MediaFileProjectRelationProject2InMediaFile::unlink(owner->getDatabase(), o0, *owner);
}
void Project::InmediafilesHandle::del(const litesql::Expr& expr) {
    MediaFileProjectRelationProject2InMediaFile::del(owner->getDatabase(), expr && MediaFileProjectRelationProject2InMediaFile::Project == owner->id);
}
litesql::DataSource<MediaFile> Project::InmediafilesHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return MediaFileProjectRelationProject2InMediaFile::get<MediaFile>(owner->getDatabase(), expr, (MediaFileProjectRelationProject2InMediaFile::Project == owner->id) && srcExpr);
}
litesql::DataSource<MediaFileProjectRelationProject2InMediaFile::Row> Project::InmediafilesHandle::getRows(const litesql::Expr& expr) {
    return MediaFileProjectRelationProject2InMediaFile::getRows(owner->getDatabase(), expr && (MediaFileProjectRelationProject2InMediaFile::Project == owner->id));
}
Project::JobsHandle::JobsHandle(const Project& owner)
         : litesql::RelationHandle<Project>(owner) {
}
void Project::JobsHandle::link(const Job& o0) {
    JobProjectRelationProject2Job::link(owner->getDatabase(), o0, *owner);
}
void Project::JobsHandle::unlink(const Job& o0) {
    JobProjectRelationProject2Job::unlink(owner->getDatabase(), o0, *owner);
}
void Project::JobsHandle::del(const litesql::Expr& expr) {
    JobProjectRelationProject2Job::del(owner->getDatabase(), expr && JobProjectRelationProject2Job::Project == owner->id);
}
litesql::DataSource<Job> Project::JobsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobProjectRelationProject2Job::get<Job>(owner->getDatabase(), expr, (JobProjectRelationProject2Job::Project == owner->id) && srcExpr);
}
litesql::DataSource<JobProjectRelationProject2Job::Row> Project::JobsHandle::getRows(const litesql::Expr& expr) {
    return JobProjectRelationProject2Job::getRows(owner->getDatabase(), expr && (JobProjectRelationProject2Job::Project == owner->id));
}
Project::PresetsHandle::PresetsHandle(const Project& owner)
         : litesql::RelationHandle<Project>(owner) {
}
void Project::PresetsHandle::link(const Preset& o0) {
    PresetProjectRelationProject2Preset::link(owner->getDatabase(), o0, *owner);
}
void Project::PresetsHandle::unlink(const Preset& o0) {
    PresetProjectRelationProject2Preset::unlink(owner->getDatabase(), o0, *owner);
}
void Project::PresetsHandle::del(const litesql::Expr& expr) {
    PresetProjectRelationProject2Preset::del(owner->getDatabase(), expr && PresetProjectRelationProject2Preset::Project == owner->id);
}
litesql::DataSource<Preset> Project::PresetsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return PresetProjectRelationProject2Preset::get<Preset>(owner->getDatabase(), expr, (PresetProjectRelationProject2Preset::Project == owner->id) && srcExpr);
}
litesql::DataSource<PresetProjectRelationProject2Preset::Row> Project::PresetsHandle::getRows(const litesql::Expr& expr) {
    return PresetProjectRelationProject2Preset::getRows(owner->getDatabase(), expr && (PresetProjectRelationProject2Preset::Project == owner->id));
}
const std::string Project::type__("Project");
const std::string Project::table__("Project_");
const std::string Project::sequence__("Project_seq");
const litesql::FieldType Project::Id("id_","INTEGER",table__);
const litesql::FieldType Project::Type("type_","TEXT",table__);
const litesql::FieldType Project::Name("name_","TEXT",table__);
const litesql::FieldType Project::Outdirectory("outdirectory_","TEXT",table__);
const litesql::FieldType Project::Status("status_","TEXT",table__);
const litesql::FieldType Project::Created("created_","INTEGER",table__);
const litesql::FieldType Project::Started("started_","INTEGER",table__);
const litesql::FieldType Project::Completed("completed_","INTEGER",table__);
void Project::defaults() {
    id = 0;
    created = 0;
    started = 0;
    completed = 0;
}
Project::Project(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name), outdirectory(Outdirectory), status(Status), created(Created), started(Started), completed(Completed) {
    defaults();
}
Project::Project(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name), outdirectory(Outdirectory), status(Status), created(Created), started(Started), completed(Completed) {
    defaults();
    size_t size = (rec.size() > 8) ? 8 : rec.size();
    switch(size) {
    case 8: completed = convert<const std::string&, litesql::Date>(rec[7]);
        completed.setModified(false);
    case 7: started = convert<const std::string&, litesql::Date>(rec[6]);
        started.setModified(false);
    case 6: created = convert<const std::string&, litesql::Date>(rec[5]);
        created.setModified(false);
    case 5: status = convert<const std::string&, std::string>(rec[4]);
        status.setModified(false);
    case 4: outdirectory = convert<const std::string&, std::string>(rec[3]);
        outdirectory.setModified(false);
    case 3: name = convert<const std::string&, std::string>(rec[2]);
        name.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Project::Project(const Project& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name), outdirectory(obj.outdirectory), status(obj.status), created(obj.created), started(obj.started), completed(obj.completed) {
}
const Project& Project::operator=(const Project& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
        outdirectory = obj.outdirectory;
        status = obj.status;
        created = obj.created;
        started = obj.started;
        completed = obj.completed;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Project::OutmediafilesHandle Project::outmediafiles() {
    return Project::OutmediafilesHandle(*this);
}
Project::InmediafilesHandle Project::inmediafiles() {
    return Project::InmediafilesHandle(*this);
}
Project::JobsHandle Project::jobs() {
    return Project::JobsHandle(*this);
}
Project::PresetsHandle Project::presets() {
    return Project::PresetsHandle(*this);
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
    fields.push_back(outdirectory.name());
    values.push_back(outdirectory);
    outdirectory.setModified(false);
    fields.push_back(status.name());
    values.push_back(status);
    status.setModified(false);
    fields.push_back(created.name());
    values.push_back(created);
    created.setModified(false);
    fields.push_back(started.name());
    values.push_back(started);
    started.setModified(false);
    fields.push_back(completed.name());
    values.push_back(completed);
    completed.setModified(false);
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
    updateField(updates, table__, outdirectory);
    updateField(updates, table__, status);
    updateField(updates, table__, created);
    updateField(updates, table__, started);
    updateField(updates, table__, completed);
}
void Project::addIDUpdates(Updates& updates) {
}
void Project::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
    ftypes.push_back(Outdirectory);
    ftypes.push_back(Status);
    ftypes.push_back(Created);
    ftypes.push_back(Started);
    ftypes.push_back(Completed);
}
void Project::delRecord() {
    deleteFromTable(table__, id);
}
void Project::delRelations() {
    MediaFileProjectRelationProject2OutMediaFile::del(*db, (MediaFileProjectRelationProject2OutMediaFile::Project == id));
    MediaFileProjectRelationProject2InMediaFile::del(*db, (MediaFileProjectRelationProject2InMediaFile::Project == id));
    JobProjectRelationProject2Job::del(*db, (JobProjectRelationProject2Job::Project == id));
    PresetProjectRelationProject2Preset::del(*db, (PresetProjectRelationProject2Preset::Project == id));
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
    np->outdirectory = outdirectory;
    np->status = status;
    np->created = created;
    np->started = started;
    np->completed = completed;
    np->inDatabase = inDatabase;
    return auto_ptr<Project>(np);
}
std::ostream & operator<<(std::ostream& os, Project o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.outdirectory.name() << " = " << o.outdirectory << std::endl;
    os << o.status.name() << " = " << o.status << std::endl;
    os << o.created.name() << " = " << o.created << std::endl;
    os << o.started.name() << " = " << o.started << std::endl;
    os << o.completed.name() << " = " << o.completed << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Job::Own::Id("id_","INTEGER","Job_");
Job::ProjectHandle::ProjectHandle(const Job& owner)
         : litesql::RelationHandle<Job>(owner) {
}
void Job::ProjectHandle::link(const Project& o0) {
    JobProjectRelationProject2Job::link(owner->getDatabase(), *owner, o0);
}
void Job::ProjectHandle::unlink(const Project& o0) {
    JobProjectRelationProject2Job::unlink(owner->getDatabase(), *owner, o0);
}
void Job::ProjectHandle::del(const litesql::Expr& expr) {
    JobProjectRelationProject2Job::del(owner->getDatabase(), expr && JobProjectRelationProject2Job::Job == owner->id);
}
litesql::DataSource<Project> Job::ProjectHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobProjectRelationProject2Job::get<Project>(owner->getDatabase(), expr, (JobProjectRelationProject2Job::Job == owner->id) && srcExpr);
}
litesql::DataSource<JobProjectRelationProject2Job::Row> Job::ProjectHandle::getRows(const litesql::Expr& expr) {
    return JobProjectRelationProject2Job::getRows(owner->getDatabase(), expr && (JobProjectRelationProject2Job::Job == owner->id));
}
Job::LogsHandle::LogsHandle(const Job& owner)
         : litesql::RelationHandle<Job>(owner) {
}
void Job::LogsHandle::link(const JobLog& o0) {
    JobJobLogRelationJob2JobLog::link(owner->getDatabase(), *owner, o0);
}
void Job::LogsHandle::unlink(const JobLog& o0) {
    JobJobLogRelationJob2JobLog::unlink(owner->getDatabase(), *owner, o0);
}
void Job::LogsHandle::del(const litesql::Expr& expr) {
    JobJobLogRelationJob2JobLog::del(owner->getDatabase(), expr && JobJobLogRelationJob2JobLog::Job == owner->id);
}
litesql::DataSource<JobLog> Job::LogsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobJobLogRelationJob2JobLog::get<JobLog>(owner->getDatabase(), expr, (JobJobLogRelationJob2JobLog::Job == owner->id) && srcExpr);
}
litesql::DataSource<JobJobLogRelationJob2JobLog::Row> Job::LogsHandle::getRows(const litesql::Expr& expr) {
    return JobJobLogRelationJob2JobLog::getRows(owner->getDatabase(), expr && (JobJobLogRelationJob2JobLog::Job == owner->id));
}
const std::string Job::type__("Job");
const std::string Job::table__("Job_");
const std::string Job::sequence__("Job_seq");
const litesql::FieldType Job::Id("id_","INTEGER",table__);
const litesql::FieldType Job::Type("type_","TEXT",table__);
const litesql::FieldType Job::Created("created_","INTEGER",table__);
const litesql::FieldType Job::Begintime("begintime_","INTEGER",table__);
const litesql::FieldType Job::Endtime("endtime_","INTEGER",table__);
const litesql::FieldType Job::Status("status_","TEXT",table__);
const litesql::FieldType Job::Infile("infile_","TEXT",table__);
const litesql::FieldType Job::Outfile("outfile_","TEXT",table__);
const litesql::FieldType Job::Starttime("starttime_","DOUBLE",table__);
const litesql::FieldType Job::Duration("duration_","DOUBLE",table__);
const litesql::FieldType Job::Progress("progress_","INTEGER",table__);
void Job::defaults() {
    id = 0;
    created = -1;
    begintime = -1;
    endtime = -1;
    starttime = 0.0;
    duration = 0.0;
    progress = 0;
}
Job::Job(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), created(Created), begintime(Begintime), endtime(Endtime), status(Status), infile(Infile), outfile(Outfile), starttime(Starttime), duration(Duration), progress(Progress) {
    defaults();
}
Job::Job(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), created(Created), begintime(Begintime), endtime(Endtime), status(Status), infile(Infile), outfile(Outfile), starttime(Starttime), duration(Duration), progress(Progress) {
    defaults();
    size_t size = (rec.size() > 11) ? 11 : rec.size();
    switch(size) {
    case 11: progress = convert<const std::string&, int>(rec[10]);
        progress.setModified(false);
    case 10: duration = convert<const std::string&, double>(rec[9]);
        duration.setModified(false);
    case 9: starttime = convert<const std::string&, double>(rec[8]);
        starttime.setModified(false);
    case 8: outfile = convert<const std::string&, std::string>(rec[7]);
        outfile.setModified(false);
    case 7: infile = convert<const std::string&, std::string>(rec[6]);
        infile.setModified(false);
    case 6: status = convert<const std::string&, std::string>(rec[5]);
        status.setModified(false);
    case 5: endtime = convert<const std::string&, litesql::DateTime>(rec[4]);
        endtime.setModified(false);
    case 4: begintime = convert<const std::string&, litesql::DateTime>(rec[3]);
        begintime.setModified(false);
    case 3: created = convert<const std::string&, litesql::DateTime>(rec[2]);
        created.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Job::Job(const Job& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), created(obj.created), begintime(obj.begintime), endtime(obj.endtime), status(obj.status), infile(obj.infile), outfile(obj.outfile), starttime(obj.starttime), duration(obj.duration), progress(obj.progress) {
}
const Job& Job::operator=(const Job& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        created = obj.created;
        begintime = obj.begintime;
        endtime = obj.endtime;
        status = obj.status;
        infile = obj.infile;
        outfile = obj.outfile;
        starttime = obj.starttime;
        duration = obj.duration;
        progress = obj.progress;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Job::ProjectHandle Job::project() {
    return Job::ProjectHandle(*this);
}
Job::LogsHandle Job::logs() {
    return Job::LogsHandle(*this);
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
    fields.push_back(created.name());
    values.push_back(created);
    created.setModified(false);
    fields.push_back(begintime.name());
    values.push_back(begintime);
    begintime.setModified(false);
    fields.push_back(endtime.name());
    values.push_back(endtime);
    endtime.setModified(false);
    fields.push_back(status.name());
    values.push_back(status);
    status.setModified(false);
    fields.push_back(infile.name());
    values.push_back(infile);
    infile.setModified(false);
    fields.push_back(outfile.name());
    values.push_back(outfile);
    outfile.setModified(false);
    fields.push_back(starttime.name());
    values.push_back(starttime);
    starttime.setModified(false);
    fields.push_back(duration.name());
    values.push_back(duration);
    duration.setModified(false);
    fields.push_back(progress.name());
    values.push_back(progress);
    progress.setModified(false);
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
    updateField(updates, table__, created);
    updateField(updates, table__, begintime);
    updateField(updates, table__, endtime);
    updateField(updates, table__, status);
    updateField(updates, table__, infile);
    updateField(updates, table__, outfile);
    updateField(updates, table__, starttime);
    updateField(updates, table__, duration);
    updateField(updates, table__, progress);
}
void Job::addIDUpdates(Updates& updates) {
}
void Job::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Created);
    ftypes.push_back(Begintime);
    ftypes.push_back(Endtime);
    ftypes.push_back(Status);
    ftypes.push_back(Infile);
    ftypes.push_back(Outfile);
    ftypes.push_back(Starttime);
    ftypes.push_back(Duration);
    ftypes.push_back(Progress);
}
void Job::delRecord() {
    deleteFromTable(table__, id);
}
void Job::delRelations() {
    JobProjectRelationProject2Job::del(*db, (JobProjectRelationProject2Job::Job == id));
    JobJobLogRelationJob2JobLog::del(*db, (JobJobLogRelationJob2JobLog::Job == id));
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
    np->created = created;
    np->begintime = begintime;
    np->endtime = endtime;
    np->status = status;
    np->infile = infile;
    np->outfile = outfile;
    np->starttime = starttime;
    np->duration = duration;
    np->progress = progress;
    np->inDatabase = inDatabase;
    return auto_ptr<Job>(np);
}
std::ostream & operator<<(std::ostream& os, Job o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.created.name() << " = " << o.created << std::endl;
    os << o.begintime.name() << " = " << o.begintime << std::endl;
    os << o.endtime.name() << " = " << o.endtime << std::endl;
    os << o.status.name() << " = " << o.status << std::endl;
    os << o.infile.name() << " = " << o.infile << std::endl;
    os << o.outfile.name() << " = " << o.outfile << std::endl;
    os << o.starttime.name() << " = " << o.starttime << std::endl;
    os << o.duration.name() << " = " << o.duration << std::endl;
    os << o.progress.name() << " = " << o.progress << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType JobLog::Own::Id("id_","INTEGER","JobLog_");
JobLog::JobHandle::JobHandle(const JobLog& owner)
         : litesql::RelationHandle<JobLog>(owner) {
}
void JobLog::JobHandle::link(const Job& o0) {
    JobJobLogRelationJob2JobLog::link(owner->getDatabase(), o0, *owner);
}
void JobLog::JobHandle::unlink(const Job& o0) {
    JobJobLogRelationJob2JobLog::unlink(owner->getDatabase(), o0, *owner);
}
void JobLog::JobHandle::del(const litesql::Expr& expr) {
    JobJobLogRelationJob2JobLog::del(owner->getDatabase(), expr && JobJobLogRelationJob2JobLog::JobLog == owner->id);
}
litesql::DataSource<Job> JobLog::JobHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobJobLogRelationJob2JobLog::get<Job>(owner->getDatabase(), expr, (JobJobLogRelationJob2JobLog::JobLog == owner->id) && srcExpr);
}
litesql::DataSource<JobJobLogRelationJob2JobLog::Row> JobLog::JobHandle::getRows(const litesql::Expr& expr) {
    return JobJobLogRelationJob2JobLog::getRows(owner->getDatabase(), expr && (JobJobLogRelationJob2JobLog::JobLog == owner->id));
}
const std::string JobLog::type__("JobLog");
const std::string JobLog::table__("JobLog_");
const std::string JobLog::sequence__("JobLog_seq");
const litesql::FieldType JobLog::Id("id_","INTEGER",table__);
const litesql::FieldType JobLog::Type("type_","TEXT",table__);
const litesql::FieldType JobLog::Created("created_","INTEGER",table__);
const litesql::FieldType JobLog::Messagetype("messagetype_","INTEGER",table__);
const litesql::FieldType JobLog::Message("message_","TEXT",table__);
void JobLog::defaults() {
    id = 0;
    created = 0;
    messagetype = 0;
}
JobLog::JobLog(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), created(Created), messagetype(Messagetype), message(Message) {
    defaults();
}
JobLog::JobLog(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), created(Created), messagetype(Messagetype), message(Message) {
    defaults();
    size_t size = (rec.size() > 5) ? 5 : rec.size();
    switch(size) {
    case 5: message = convert<const std::string&, std::string>(rec[4]);
        message.setModified(false);
    case 4: messagetype = convert<const std::string&, int>(rec[3]);
        messagetype.setModified(false);
    case 3: created = convert<const std::string&, litesql::DateTime>(rec[2]);
        created.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
JobLog::JobLog(const JobLog& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), created(obj.created), messagetype(obj.messagetype), message(obj.message) {
}
const JobLog& JobLog::operator=(const JobLog& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        created = obj.created;
        messagetype = obj.messagetype;
        message = obj.message;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
JobLog::JobHandle JobLog::job() {
    return JobLog::JobHandle(*this);
}
std::string JobLog::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(created.name());
    values.push_back(created);
    created.setModified(false);
    fields.push_back(messagetype.name());
    values.push_back(messagetype);
    messagetype.setModified(false);
    fields.push_back(message.name());
    values.push_back(message);
    message.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void JobLog::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void JobLog::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, created);
    updateField(updates, table__, messagetype);
    updateField(updates, table__, message);
}
void JobLog::addIDUpdates(Updates& updates) {
}
void JobLog::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Created);
    ftypes.push_back(Messagetype);
    ftypes.push_back(Message);
}
void JobLog::delRecord() {
    deleteFromTable(table__, id);
}
void JobLog::delRelations() {
    JobJobLogRelationJob2JobLog::del(*db, (JobJobLogRelationJob2JobLog::JobLog == id));
}
void JobLog::update() {
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
void JobLog::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<JobLog> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool JobLog::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<JobLog> JobLog::upcast() {
    return auto_ptr<JobLog>(new JobLog(*this));
}
std::auto_ptr<JobLog> JobLog::upcastCopy() {
    JobLog* np = new JobLog(*this);
    np->id = id;
    np->type = type;
    np->created = created;
    np->messagetype = messagetype;
    np->message = message;
    np->inDatabase = inDatabase;
    return auto_ptr<JobLog>(np);
}
std::ostream & operator<<(std::ostream& os, JobLog o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.created.name() << " = " << o.created << std::endl;
    os << o.messagetype.name() << " = " << o.messagetype << std::endl;
    os << o.message.name() << " = " << o.message << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Preset::Own::Id("id_","INTEGER","Preset_");
Preset::ProjectHandle::ProjectHandle(const Preset& owner)
         : litesql::RelationHandle<Preset>(owner) {
}
void Preset::ProjectHandle::link(const Project& o0) {
    PresetProjectRelationProject2Preset::link(owner->getDatabase(), *owner, o0);
}
void Preset::ProjectHandle::unlink(const Project& o0) {
    PresetProjectRelationProject2Preset::unlink(owner->getDatabase(), *owner, o0);
}
void Preset::ProjectHandle::del(const litesql::Expr& expr) {
    PresetProjectRelationProject2Preset::del(owner->getDatabase(), expr && PresetProjectRelationProject2Preset::Preset == owner->id);
}
litesql::DataSource<Project> Preset::ProjectHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return PresetProjectRelationProject2Preset::get<Project>(owner->getDatabase(), expr, (PresetProjectRelationProject2Preset::Preset == owner->id) && srcExpr);
}
litesql::DataSource<PresetProjectRelationProject2Preset::Row> Preset::ProjectHandle::getRows(const litesql::Expr& expr) {
    return PresetProjectRelationProject2Preset::getRows(owner->getDatabase(), expr && (PresetProjectRelationProject2Preset::Preset == owner->id));
}
Preset::CodecpresetsHandle::CodecpresetsHandle(const Preset& owner)
         : litesql::RelationHandle<Preset>(owner) {
}
void Preset::CodecpresetsHandle::link(const CodecPreset& o0) {
    CodecPresetPresetRelationPreset2CodecPreset::link(owner->getDatabase(), o0, *owner);
}
void Preset::CodecpresetsHandle::unlink(const CodecPreset& o0) {
    CodecPresetPresetRelationPreset2CodecPreset::unlink(owner->getDatabase(), o0, *owner);
}
void Preset::CodecpresetsHandle::del(const litesql::Expr& expr) {
    CodecPresetPresetRelationPreset2CodecPreset::del(owner->getDatabase(), expr && CodecPresetPresetRelationPreset2CodecPreset::Preset == owner->id);
}
litesql::DataSource<CodecPreset> Preset::CodecpresetsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return CodecPresetPresetRelationPreset2CodecPreset::get<CodecPreset>(owner->getDatabase(), expr, (CodecPresetPresetRelationPreset2CodecPreset::Preset == owner->id) && srcExpr);
}
litesql::DataSource<CodecPresetPresetRelationPreset2CodecPreset::Row> Preset::CodecpresetsHandle::getRows(const litesql::Expr& expr) {
    return CodecPresetPresetRelationPreset2CodecPreset::getRows(owner->getDatabase(), expr && (CodecPresetPresetRelationPreset2CodecPreset::Preset == owner->id));
}
Preset::FiltersHandle::FiltersHandle(const Preset& owner)
         : litesql::RelationHandle<Preset>(owner) {
}
void Preset::FiltersHandle::link(const Filter& o0) {
    FilterPresetRelationPreset2Filter::link(owner->getDatabase(), o0, *owner);
}
void Preset::FiltersHandle::unlink(const Filter& o0) {
    FilterPresetRelationPreset2Filter::unlink(owner->getDatabase(), o0, *owner);
}
void Preset::FiltersHandle::del(const litesql::Expr& expr) {
    FilterPresetRelationPreset2Filter::del(owner->getDatabase(), expr && FilterPresetRelationPreset2Filter::Preset == owner->id);
}
litesql::DataSource<Filter> Preset::FiltersHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FilterPresetRelationPreset2Filter::get<Filter>(owner->getDatabase(), expr, (FilterPresetRelationPreset2Filter::Preset == owner->id) && srcExpr);
}
litesql::DataSource<FilterPresetRelationPreset2Filter::Row> Preset::FiltersHandle::getRows(const litesql::Expr& expr) {
    return FilterPresetRelationPreset2Filter::getRows(owner->getDatabase(), expr && (FilterPresetRelationPreset2Filter::Preset == owner->id));
}
Preset::FormatHandle::FormatHandle(const Preset& owner)
         : litesql::RelationHandle<Preset>(owner) {
}
void Preset::FormatHandle::link(const Format& o0) {
    FormatPresetRelationPreset2Format::link(owner->getDatabase(), o0, *owner);
}
void Preset::FormatHandle::unlink(const Format& o0) {
    FormatPresetRelationPreset2Format::unlink(owner->getDatabase(), o0, *owner);
}
void Preset::FormatHandle::del(const litesql::Expr& expr) {
    FormatPresetRelationPreset2Format::del(owner->getDatabase(), expr && FormatPresetRelationPreset2Format::Preset == owner->id);
}
litesql::DataSource<Format> Preset::FormatHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FormatPresetRelationPreset2Format::get<Format>(owner->getDatabase(), expr, (FormatPresetRelationPreset2Format::Preset == owner->id) && srcExpr);
}
litesql::DataSource<FormatPresetRelationPreset2Format::Row> Preset::FormatHandle::getRows(const litesql::Expr& expr) {
    return FormatPresetRelationPreset2Format::getRows(owner->getDatabase(), expr && (FormatPresetRelationPreset2Format::Preset == owner->id));
}
const std::string Preset::type__("Preset");
const std::string Preset::table__("Preset_");
const std::string Preset::sequence__("Preset_seq");
const litesql::FieldType Preset::Id("id_","INTEGER",table__);
const litesql::FieldType Preset::Type("type_","TEXT",table__);
const litesql::FieldType Preset::Presetname("presetname_","TEXT",table__);
const litesql::FieldType Preset::Description("description_","TEXT",table__);
const litesql::FieldType Preset::Presettype("presettype_","INTEGER",table__);
const litesql::FieldType Preset::Created("created_","INTEGER",table__);
void Preset::defaults() {
    id = 0;
    presettype = 0;
    created = 1;
}
Preset::Preset(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), presetname(Presetname), description(Description), presettype(Presettype), created(Created) {
    defaults();
}
Preset::Preset(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), presetname(Presetname), description(Description), presettype(Presettype), created(Created) {
    defaults();
    size_t size = (rec.size() > 6) ? 6 : rec.size();
    switch(size) {
    case 6: created = convert<const std::string&, litesql::DateTime>(rec[5]);
        created.setModified(false);
    case 5: presettype = convert<const std::string&, int>(rec[4]);
        presettype.setModified(false);
    case 4: description = convert<const std::string&, std::string>(rec[3]);
        description.setModified(false);
    case 3: presetname = convert<const std::string&, std::string>(rec[2]);
        presetname.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Preset::Preset(const Preset& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), presetname(obj.presetname), description(obj.description), presettype(obj.presettype), created(obj.created) {
}
const Preset& Preset::operator=(const Preset& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        presetname = obj.presetname;
        description = obj.description;
        presettype = obj.presettype;
        created = obj.created;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Preset::ProjectHandle Preset::project() {
    return Preset::ProjectHandle(*this);
}
Preset::CodecpresetsHandle Preset::codecpresets() {
    return Preset::CodecpresetsHandle(*this);
}
Preset::FiltersHandle Preset::filters() {
    return Preset::FiltersHandle(*this);
}
Preset::FormatHandle Preset::format() {
    return Preset::FormatHandle(*this);
}
std::string Preset::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(presetname.name());
    values.push_back(presetname);
    presetname.setModified(false);
    fields.push_back(description.name());
    values.push_back(description);
    description.setModified(false);
    fields.push_back(presettype.name());
    values.push_back(presettype);
    presettype.setModified(false);
    fields.push_back(created.name());
    values.push_back(created);
    created.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Preset::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Preset::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, presetname);
    updateField(updates, table__, description);
    updateField(updates, table__, presettype);
    updateField(updates, table__, created);
}
void Preset::addIDUpdates(Updates& updates) {
}
void Preset::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Presetname);
    ftypes.push_back(Description);
    ftypes.push_back(Presettype);
    ftypes.push_back(Created);
}
void Preset::delRecord() {
    deleteFromTable(table__, id);
}
void Preset::delRelations() {
    PresetProjectRelationProject2Preset::del(*db, (PresetProjectRelationProject2Preset::Preset == id));
    CodecPresetPresetRelationPreset2CodecPreset::del(*db, (CodecPresetPresetRelationPreset2CodecPreset::Preset == id));
    FilterPresetRelationPreset2Filter::del(*db, (FilterPresetRelationPreset2Filter::Preset == id));
    FormatPresetRelationPreset2Format::del(*db, (FormatPresetRelationPreset2Format::Preset == id));
}
void Preset::update() {
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
void Preset::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<Preset> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Preset::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<Preset> Preset::upcast() {
    return auto_ptr<Preset>(new Preset(*this));
}
std::auto_ptr<Preset> Preset::upcastCopy() {
    Preset* np = new Preset(*this);
    np->id = id;
    np->type = type;
    np->presetname = presetname;
    np->description = description;
    np->presettype = presettype;
    np->created = created;
    np->inDatabase = inDatabase;
    return auto_ptr<Preset>(np);
}
std::ostream & operator<<(std::ostream& os, Preset o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.presetname.name() << " = " << o.presetname << std::endl;
    os << o.description.name() << " = " << o.description << std::endl;
    os << o.presettype.name() << " = " << o.presettype << std::endl;
    os << o.created.name() << " = " << o.created << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType CodecPreset::Own::Id("id_","INTEGER","CodecPreset_");
CodecPreset::PresetHandle::PresetHandle(const CodecPreset& owner)
         : litesql::RelationHandle<CodecPreset>(owner) {
}
void CodecPreset::PresetHandle::link(const Preset& o0) {
    CodecPresetPresetRelationPreset2CodecPreset::link(owner->getDatabase(), *owner, o0);
}
void CodecPreset::PresetHandle::unlink(const Preset& o0) {
    CodecPresetPresetRelationPreset2CodecPreset::unlink(owner->getDatabase(), *owner, o0);
}
void CodecPreset::PresetHandle::del(const litesql::Expr& expr) {
    CodecPresetPresetRelationPreset2CodecPreset::del(owner->getDatabase(), expr && CodecPresetPresetRelationPreset2CodecPreset::CodecPreset == owner->id);
}
litesql::DataSource<Preset> CodecPreset::PresetHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return CodecPresetPresetRelationPreset2CodecPreset::get<Preset>(owner->getDatabase(), expr, (CodecPresetPresetRelationPreset2CodecPreset::CodecPreset == owner->id) && srcExpr);
}
litesql::DataSource<CodecPresetPresetRelationPreset2CodecPreset::Row> CodecPreset::PresetHandle::getRows(const litesql::Expr& expr) {
    return CodecPresetPresetRelationPreset2CodecPreset::getRows(owner->getDatabase(), expr && (CodecPresetPresetRelationPreset2CodecPreset::CodecPreset == owner->id));
}
const std::string CodecPreset::type__("CodecPreset");
const std::string CodecPreset::table__("CodecPreset_");
const std::string CodecPreset::sequence__("CodecPreset_seq");
const litesql::FieldType CodecPreset::Id("id_","INTEGER",table__);
const litesql::FieldType CodecPreset::Type("type_","TEXT",table__);
const litesql::FieldType CodecPreset::Presetname("presetname_","TEXT",table__);
const litesql::FieldType CodecPreset::Description("description_","TEXT",table__);
const litesql::FieldType CodecPreset::Presettype("presettype_","INTEGER",table__);
const litesql::FieldType CodecPreset::Created("created_","INTEGER",table__);
void CodecPreset::defaults() {
    id = 0;
    presettype = 0;
    created = 1;
}
CodecPreset::CodecPreset(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), presetname(Presetname), description(Description), presettype(Presettype), created(Created) {
    defaults();
}
CodecPreset::CodecPreset(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), presetname(Presetname), description(Description), presettype(Presettype), created(Created) {
    defaults();
    size_t size = (rec.size() > 6) ? 6 : rec.size();
    switch(size) {
    case 6: created = convert<const std::string&, litesql::DateTime>(rec[5]);
        created.setModified(false);
    case 5: presettype = convert<const std::string&, int>(rec[4]);
        presettype.setModified(false);
    case 4: description = convert<const std::string&, std::string>(rec[3]);
        description.setModified(false);
    case 3: presetname = convert<const std::string&, std::string>(rec[2]);
        presetname.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
CodecPreset::CodecPreset(const CodecPreset& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), presetname(obj.presetname), description(obj.description), presettype(obj.presettype), created(obj.created) {
}
const CodecPreset& CodecPreset::operator=(const CodecPreset& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        presetname = obj.presetname;
        description = obj.description;
        presettype = obj.presettype;
        created = obj.created;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
CodecPreset::PresetHandle CodecPreset::preset() {
    return CodecPreset::PresetHandle(*this);
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
    fields.push_back(presetname.name());
    values.push_back(presetname);
    presetname.setModified(false);
    fields.push_back(description.name());
    values.push_back(description);
    description.setModified(false);
    fields.push_back(presettype.name());
    values.push_back(presettype);
    presettype.setModified(false);
    fields.push_back(created.name());
    values.push_back(created);
    created.setModified(false);
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
    updateField(updates, table__, presetname);
    updateField(updates, table__, description);
    updateField(updates, table__, presettype);
    updateField(updates, table__, created);
}
void CodecPreset::addIDUpdates(Updates& updates) {
}
void CodecPreset::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Presetname);
    ftypes.push_back(Description);
    ftypes.push_back(Presettype);
    ftypes.push_back(Created);
}
void CodecPreset::delRecord() {
    deleteFromTable(table__, id);
}
void CodecPreset::delRelations() {
    CodecPresetPresetRelationPreset2CodecPreset::del(*db, (CodecPresetPresetRelationPreset2CodecPreset::CodecPreset == id));
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
    np->presetname = presetname;
    np->description = description;
    np->presettype = presettype;
    np->created = created;
    np->inDatabase = inDatabase;
    return auto_ptr<CodecPreset>(np);
}
std::ostream & operator<<(std::ostream& os, CodecPreset o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.presetname.name() << " = " << o.presetname << std::endl;
    os << o.description.name() << " = " << o.description << std::endl;
    os << o.presettype.name() << " = " << o.presettype << std::endl;
    os << o.created.name() << " = " << o.created << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Filter::Own::Id("id_","INTEGER","Filter_");
Filter::PresetHandle::PresetHandle(const Filter& owner)
         : litesql::RelationHandle<Filter>(owner) {
}
void Filter::PresetHandle::link(const Preset& o0) {
    FilterPresetRelationPreset2Filter::link(owner->getDatabase(), *owner, o0);
}
void Filter::PresetHandle::unlink(const Preset& o0) {
    FilterPresetRelationPreset2Filter::unlink(owner->getDatabase(), *owner, o0);
}
void Filter::PresetHandle::del(const litesql::Expr& expr) {
    FilterPresetRelationPreset2Filter::del(owner->getDatabase(), expr && FilterPresetRelationPreset2Filter::Filter == owner->id);
}
litesql::DataSource<Preset> Filter::PresetHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FilterPresetRelationPreset2Filter::get<Preset>(owner->getDatabase(), expr, (FilterPresetRelationPreset2Filter::Filter == owner->id) && srcExpr);
}
litesql::DataSource<FilterPresetRelationPreset2Filter::Row> Filter::PresetHandle::getRows(const litesql::Expr& expr) {
    return FilterPresetRelationPreset2Filter::getRows(owner->getDatabase(), expr && (FilterPresetRelationPreset2Filter::Filter == owner->id));
}
Filter::ParameterHandle::ParameterHandle(const Filter& owner)
         : litesql::RelationHandle<Filter>(owner) {
}
void Filter::ParameterHandle::link(const FilterParameter& o0) {
    FilterFilterParameterRelationFilter2FilterParameter::link(owner->getDatabase(), *owner, o0);
}
void Filter::ParameterHandle::unlink(const FilterParameter& o0) {
    FilterFilterParameterRelationFilter2FilterParameter::unlink(owner->getDatabase(), *owner, o0);
}
void Filter::ParameterHandle::del(const litesql::Expr& expr) {
    FilterFilterParameterRelationFilter2FilterParameter::del(owner->getDatabase(), expr && FilterFilterParameterRelationFilter2FilterParameter::Filter == owner->id);
}
litesql::DataSource<FilterParameter> Filter::ParameterHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FilterFilterParameterRelationFilter2FilterParameter::get<FilterParameter>(owner->getDatabase(), expr, (FilterFilterParameterRelationFilter2FilterParameter::Filter == owner->id) && srcExpr);
}
litesql::DataSource<FilterFilterParameterRelationFilter2FilterParameter::Row> Filter::ParameterHandle::getRows(const litesql::Expr& expr) {
    return FilterFilterParameterRelationFilter2FilterParameter::getRows(owner->getDatabase(), expr && (FilterFilterParameterRelationFilter2FilterParameter::Filter == owner->id));
}
const std::string Filter::type__("Filter");
const std::string Filter::table__("Filter_");
const std::string Filter::sequence__("Filter_seq");
const litesql::FieldType Filter::Id("id_","INTEGER",table__);
const litesql::FieldType Filter::Type("type_","TEXT",table__);
const litesql::FieldType Filter::Filterid("filterid_","TEXT",table__);
const litesql::FieldType Filter::Filtername("filtername_","TEXT",table__);
void Filter::defaults() {
    id = 0;
}
Filter::Filter(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), filterid(Filterid), filtername(Filtername) {
    defaults();
}
Filter::Filter(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), filterid(Filterid), filtername(Filtername) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: filtername = convert<const std::string&, std::string>(rec[3]);
        filtername.setModified(false);
    case 3: filterid = convert<const std::string&, std::string>(rec[2]);
        filterid.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Filter::Filter(const Filter& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), filterid(obj.filterid), filtername(obj.filtername) {
}
const Filter& Filter::operator=(const Filter& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        filterid = obj.filterid;
        filtername = obj.filtername;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Filter::PresetHandle Filter::preset() {
    return Filter::PresetHandle(*this);
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
    fields.push_back(filterid.name());
    values.push_back(filterid);
    filterid.setModified(false);
    fields.push_back(filtername.name());
    values.push_back(filtername);
    filtername.setModified(false);
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
    updateField(updates, table__, filterid);
    updateField(updates, table__, filtername);
}
void Filter::addIDUpdates(Updates& updates) {
}
void Filter::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Filterid);
    ftypes.push_back(Filtername);
}
void Filter::delRecord() {
    deleteFromTable(table__, id);
}
void Filter::delRelations() {
    FilterPresetRelationPreset2Filter::del(*db, (FilterPresetRelationPreset2Filter::Filter == id));
    FilterFilterParameterRelationFilter2FilterParameter::del(*db, (FilterFilterParameterRelationFilter2FilterParameter::Filter == id));
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
    np->filterid = filterid;
    np->filtername = filtername;
    np->inDatabase = inDatabase;
    return auto_ptr<Filter>(np);
}
std::ostream & operator<<(std::ostream& os, Filter o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.filterid.name() << " = " << o.filterid << std::endl;
    os << o.filtername.name() << " = " << o.filtername << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType FilterParameter::Own::Id("id_","INTEGER","FilterParameter_");
FilterParameter::FilterHandle::FilterHandle(const FilterParameter& owner)
         : litesql::RelationHandle<FilterParameter>(owner) {
}
void FilterParameter::FilterHandle::link(const Filter& o0) {
    FilterFilterParameterRelationFilter2FilterParameter::link(owner->getDatabase(), o0, *owner);
}
void FilterParameter::FilterHandle::unlink(const Filter& o0) {
    FilterFilterParameterRelationFilter2FilterParameter::unlink(owner->getDatabase(), o0, *owner);
}
void FilterParameter::FilterHandle::del(const litesql::Expr& expr) {
    FilterFilterParameterRelationFilter2FilterParameter::del(owner->getDatabase(), expr && FilterFilterParameterRelationFilter2FilterParameter::FilterParameter == owner->id);
}
litesql::DataSource<Filter> FilterParameter::FilterHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FilterFilterParameterRelationFilter2FilterParameter::get<Filter>(owner->getDatabase(), expr, (FilterFilterParameterRelationFilter2FilterParameter::FilterParameter == owner->id) && srcExpr);
}
litesql::DataSource<FilterFilterParameterRelationFilter2FilterParameter::Row> FilterParameter::FilterHandle::getRows(const litesql::Expr& expr) {
    return FilterFilterParameterRelationFilter2FilterParameter::getRows(owner->getDatabase(), expr && (FilterFilterParameterRelationFilter2FilterParameter::FilterParameter == owner->id));
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
    FilterFilterParameterRelationFilter2FilterParameter::del(*db, (FilterFilterParameterRelationFilter2FilterParameter::FilterParameter == id));
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
const litesql::FieldType Format::Own::Id("id_","INTEGER","Format_");
Format::PresetHandle::PresetHandle(const Format& owner)
         : litesql::RelationHandle<Format>(owner) {
}
void Format::PresetHandle::link(const Preset& o0) {
    FormatPresetRelationPreset2Format::link(owner->getDatabase(), *owner, o0);
}
void Format::PresetHandle::unlink(const Preset& o0) {
    FormatPresetRelationPreset2Format::unlink(owner->getDatabase(), *owner, o0);
}
void Format::PresetHandle::del(const litesql::Expr& expr) {
    FormatPresetRelationPreset2Format::del(owner->getDatabase(), expr && FormatPresetRelationPreset2Format::Format == owner->id);
}
litesql::DataSource<Preset> Format::PresetHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FormatPresetRelationPreset2Format::get<Preset>(owner->getDatabase(), expr, (FormatPresetRelationPreset2Format::Format == owner->id) && srcExpr);
}
litesql::DataSource<FormatPresetRelationPreset2Format::Row> Format::PresetHandle::getRows(const litesql::Expr& expr) {
    return FormatPresetRelationPreset2Format::getRows(owner->getDatabase(), expr && (FormatPresetRelationPreset2Format::Format == owner->id));
}
Format::ParameterHandle::ParameterHandle(const Format& owner)
         : litesql::RelationHandle<Format>(owner) {
}
void Format::ParameterHandle::link(const FormatParameter& o0) {
    FormatFormatParameterRelationFormat2FormatParameter::link(owner->getDatabase(), *owner, o0);
}
void Format::ParameterHandle::unlink(const FormatParameter& o0) {
    FormatFormatParameterRelationFormat2FormatParameter::unlink(owner->getDatabase(), *owner, o0);
}
void Format::ParameterHandle::del(const litesql::Expr& expr) {
    FormatFormatParameterRelationFormat2FormatParameter::del(owner->getDatabase(), expr && FormatFormatParameterRelationFormat2FormatParameter::Format == owner->id);
}
litesql::DataSource<FormatParameter> Format::ParameterHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FormatFormatParameterRelationFormat2FormatParameter::get<FormatParameter>(owner->getDatabase(), expr, (FormatFormatParameterRelationFormat2FormatParameter::Format == owner->id) && srcExpr);
}
litesql::DataSource<FormatFormatParameterRelationFormat2FormatParameter::Row> Format::ParameterHandle::getRows(const litesql::Expr& expr) {
    return FormatFormatParameterRelationFormat2FormatParameter::getRows(owner->getDatabase(), expr && (FormatFormatParameterRelationFormat2FormatParameter::Format == owner->id));
}
Format::StreamHandle::StreamHandle(const Format& owner)
         : litesql::RelationHandle<Format>(owner) {
}
void Format::StreamHandle::link(const Stream& o0) {
    FormatStreamRelationStream2Format::link(owner->getDatabase(), *owner, o0);
}
void Format::StreamHandle::unlink(const Stream& o0) {
    FormatStreamRelationStream2Format::unlink(owner->getDatabase(), *owner, o0);
}
void Format::StreamHandle::del(const litesql::Expr& expr) {
    FormatStreamRelationStream2Format::del(owner->getDatabase(), expr && FormatStreamRelationStream2Format::Format == owner->id);
}
litesql::DataSource<Stream> Format::StreamHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FormatStreamRelationStream2Format::get<Stream>(owner->getDatabase(), expr, (FormatStreamRelationStream2Format::Format == owner->id) && srcExpr);
}
litesql::DataSource<FormatStreamRelationStream2Format::Row> Format::StreamHandle::getRows(const litesql::Expr& expr) {
    return FormatStreamRelationStream2Format::getRows(owner->getDatabase(), expr && (FormatStreamRelationStream2Format::Format == owner->id));
}
const std::string Format::type__("Format");
const std::string Format::table__("Format_");
const std::string Format::sequence__("Format_seq");
const litesql::FieldType Format::Id("id_","INTEGER",table__);
const litesql::FieldType Format::Type("type_","TEXT",table__);
const litesql::FieldType Format::Formattype("formattype_","INTEGER",table__);
const litesql::FieldType Format::Formatid("formatid_","INTEGER",table__);
const litesql::FieldType Format::Mimetype("mimetype_","TEXT",table__);
void Format::defaults() {
    id = 0;
    formattype = 0;
    formatid = 0;
}
Format::Format(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), formattype(Formattype), formatid(Formatid), mimetype(Mimetype) {
    defaults();
}
Format::Format(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), formattype(Formattype), formatid(Formatid), mimetype(Mimetype) {
    defaults();
    size_t size = (rec.size() > 5) ? 5 : rec.size();
    switch(size) {
    case 5: mimetype = convert<const std::string&, std::string>(rec[4]);
        mimetype.setModified(false);
    case 4: formatid = convert<const std::string&, int>(rec[3]);
        formatid.setModified(false);
    case 3: formattype = convert<const std::string&, int>(rec[2]);
        formattype.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Format::Format(const Format& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), formattype(obj.formattype), formatid(obj.formatid), mimetype(obj.mimetype) {
}
const Format& Format::operator=(const Format& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        formattype = obj.formattype;
        formatid = obj.formatid;
        mimetype = obj.mimetype;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Format::PresetHandle Format::preset() {
    return Format::PresetHandle(*this);
}
Format::ParameterHandle Format::parameter() {
    return Format::ParameterHandle(*this);
}
Format::StreamHandle Format::stream() {
    return Format::StreamHandle(*this);
}
std::string Format::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(formattype.name());
    values.push_back(formattype);
    formattype.setModified(false);
    fields.push_back(formatid.name());
    values.push_back(formatid);
    formatid.setModified(false);
    fields.push_back(mimetype.name());
    values.push_back(mimetype);
    mimetype.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Format::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Format::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, formattype);
    updateField(updates, table__, formatid);
    updateField(updates, table__, mimetype);
}
void Format::addIDUpdates(Updates& updates) {
}
void Format::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Formattype);
    ftypes.push_back(Formatid);
    ftypes.push_back(Mimetype);
}
void Format::delRecord() {
    deleteFromTable(table__, id);
}
void Format::delRelations() {
    FormatPresetRelationPreset2Format::del(*db, (FormatPresetRelationPreset2Format::Format == id));
    FormatFormatParameterRelationFormat2FormatParameter::del(*db, (FormatFormatParameterRelationFormat2FormatParameter::Format == id));
    FormatStreamRelationStream2Format::del(*db, (FormatStreamRelationStream2Format::Format == id));
}
void Format::update() {
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
void Format::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<Format> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Format::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<Format> Format::upcast() {
    return auto_ptr<Format>(new Format(*this));
}
std::auto_ptr<Format> Format::upcastCopy() {
    Format* np = new Format(*this);
    np->id = id;
    np->type = type;
    np->formattype = formattype;
    np->formatid = formatid;
    np->mimetype = mimetype;
    np->inDatabase = inDatabase;
    return auto_ptr<Format>(np);
}
std::ostream & operator<<(std::ostream& os, Format o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.formattype.name() << " = " << o.formattype << std::endl;
    os << o.formatid.name() << " = " << o.formatid << std::endl;
    os << o.mimetype.name() << " = " << o.mimetype << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType FormatParameter::Own::Id("id_","INTEGER","FormatParameter_");
FormatParameter::FormatHandle::FormatHandle(const FormatParameter& owner)
         : litesql::RelationHandle<FormatParameter>(owner) {
}
void FormatParameter::FormatHandle::link(const Format& o0) {
    FormatFormatParameterRelationFormat2FormatParameter::link(owner->getDatabase(), o0, *owner);
}
void FormatParameter::FormatHandle::unlink(const Format& o0) {
    FormatFormatParameterRelationFormat2FormatParameter::unlink(owner->getDatabase(), o0, *owner);
}
void FormatParameter::FormatHandle::del(const litesql::Expr& expr) {
    FormatFormatParameterRelationFormat2FormatParameter::del(owner->getDatabase(), expr && FormatFormatParameterRelationFormat2FormatParameter::FormatParameter == owner->id);
}
litesql::DataSource<Format> FormatParameter::FormatHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FormatFormatParameterRelationFormat2FormatParameter::get<Format>(owner->getDatabase(), expr, (FormatFormatParameterRelationFormat2FormatParameter::FormatParameter == owner->id) && srcExpr);
}
litesql::DataSource<FormatFormatParameterRelationFormat2FormatParameter::Row> FormatParameter::FormatHandle::getRows(const litesql::Expr& expr) {
    return FormatFormatParameterRelationFormat2FormatParameter::getRows(owner->getDatabase(), expr && (FormatFormatParameterRelationFormat2FormatParameter::FormatParameter == owner->id));
}
const std::string FormatParameter::type__("FormatParameter");
const std::string FormatParameter::table__("FormatParameter_");
const std::string FormatParameter::sequence__("FormatParameter_seq");
const litesql::FieldType FormatParameter::Id("id_","INTEGER",table__);
const litesql::FieldType FormatParameter::Type("type_","TEXT",table__);
const litesql::FieldType FormatParameter::Paramkey("paramkey_","TEXT",table__);
const litesql::FieldType FormatParameter::Paramvalue("paramvalue_","TEXT",table__);
void FormatParameter::defaults() {
    id = 0;
}
FormatParameter::FormatParameter(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), paramkey(Paramkey), paramvalue(Paramvalue) {
    defaults();
}
FormatParameter::FormatParameter(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), paramkey(Paramkey), paramvalue(Paramvalue) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: paramvalue = convert<const std::string&, std::string>(rec[3]);
        paramvalue.setModified(false);
    case 3: paramkey = convert<const std::string&, std::string>(rec[2]);
        paramkey.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
FormatParameter::FormatParameter(const FormatParameter& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), paramkey(obj.paramkey), paramvalue(obj.paramvalue) {
}
const FormatParameter& FormatParameter::operator=(const FormatParameter& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        paramkey = obj.paramkey;
        paramvalue = obj.paramvalue;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
FormatParameter::FormatHandle FormatParameter::format() {
    return FormatParameter::FormatHandle(*this);
}
std::string FormatParameter::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(paramkey.name());
    values.push_back(paramkey);
    paramkey.setModified(false);
    fields.push_back(paramvalue.name());
    values.push_back(paramvalue);
    paramvalue.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void FormatParameter::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void FormatParameter::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, paramkey);
    updateField(updates, table__, paramvalue);
}
void FormatParameter::addIDUpdates(Updates& updates) {
}
void FormatParameter::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Paramkey);
    ftypes.push_back(Paramvalue);
}
void FormatParameter::delRecord() {
    deleteFromTable(table__, id);
}
void FormatParameter::delRelations() {
    FormatFormatParameterRelationFormat2FormatParameter::del(*db, (FormatFormatParameterRelationFormat2FormatParameter::FormatParameter == id));
}
void FormatParameter::update() {
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
void FormatParameter::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<FormatParameter> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool FormatParameter::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<FormatParameter> FormatParameter::upcast() {
    return auto_ptr<FormatParameter>(new FormatParameter(*this));
}
std::auto_ptr<FormatParameter> FormatParameter::upcastCopy() {
    FormatParameter* np = new FormatParameter(*this);
    np->id = id;
    np->type = type;
    np->paramkey = paramkey;
    np->paramvalue = paramvalue;
    np->inDatabase = inDatabase;
    return auto_ptr<FormatParameter>(np);
}
std::ostream & operator<<(std::ostream& os, FormatParameter o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.paramkey.name() << " = " << o.paramkey << std::endl;
    os << o.paramvalue.name() << " = " << o.paramvalue << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Stream::Own::Id("id_","INTEGER","Stream_");
Stream::MediafileHandle::MediafileHandle(const Stream& owner)
         : litesql::RelationHandle<Stream>(owner) {
}
void Stream::MediafileHandle::link(const MediaFile& o0) {
    MediaFileStreamRelationMediaFile2Stream::link(owner->getDatabase(), o0, *owner);
}
void Stream::MediafileHandle::unlink(const MediaFile& o0) {
    MediaFileStreamRelationMediaFile2Stream::unlink(owner->getDatabase(), o0, *owner);
}
void Stream::MediafileHandle::del(const litesql::Expr& expr) {
    MediaFileStreamRelationMediaFile2Stream::del(owner->getDatabase(), expr && MediaFileStreamRelationMediaFile2Stream::Stream == owner->id);
}
litesql::DataSource<MediaFile> Stream::MediafileHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return MediaFileStreamRelationMediaFile2Stream::get<MediaFile>(owner->getDatabase(), expr, (MediaFileStreamRelationMediaFile2Stream::Stream == owner->id) && srcExpr);
}
litesql::DataSource<MediaFileStreamRelationMediaFile2Stream::Row> Stream::MediafileHandle::getRows(const litesql::Expr& expr) {
    return MediaFileStreamRelationMediaFile2Stream::getRows(owner->getDatabase(), expr && (MediaFileStreamRelationMediaFile2Stream::Stream == owner->id));
}
Stream::FormatHandle::FormatHandle(const Stream& owner)
         : litesql::RelationHandle<Stream>(owner) {
}
void Stream::FormatHandle::link(const Format& o0) {
    FormatStreamRelationStream2Format::link(owner->getDatabase(), o0, *owner);
}
void Stream::FormatHandle::unlink(const Format& o0) {
    FormatStreamRelationStream2Format::unlink(owner->getDatabase(), o0, *owner);
}
void Stream::FormatHandle::del(const litesql::Expr& expr) {
    FormatStreamRelationStream2Format::del(owner->getDatabase(), expr && FormatStreamRelationStream2Format::Stream == owner->id);
}
litesql::DataSource<Format> Stream::FormatHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FormatStreamRelationStream2Format::get<Format>(owner->getDatabase(), expr, (FormatStreamRelationStream2Format::Stream == owner->id) && srcExpr);
}
litesql::DataSource<FormatStreamRelationStream2Format::Row> Stream::FormatHandle::getRows(const litesql::Expr& expr) {
    return FormatStreamRelationStream2Format::getRows(owner->getDatabase(), expr && (FormatStreamRelationStream2Format::Stream == owner->id));
}
Stream::ProcessunitsHandle::ProcessunitsHandle(const Stream& owner)
         : litesql::RelationHandle<Stream>(owner) {
}
void Stream::ProcessunitsHandle::link(const ProcessUnit& o0) {
    ProcessUnitStreamRelationStream2ProcessUnit::link(owner->getDatabase(), o0, *owner);
}
void Stream::ProcessunitsHandle::unlink(const ProcessUnit& o0) {
    ProcessUnitStreamRelationStream2ProcessUnit::unlink(owner->getDatabase(), o0, *owner);
}
void Stream::ProcessunitsHandle::del(const litesql::Expr& expr) {
    ProcessUnitStreamRelationStream2ProcessUnit::del(owner->getDatabase(), expr && ProcessUnitStreamRelationStream2ProcessUnit::Stream == owner->id);
}
litesql::DataSource<ProcessUnit> Stream::ProcessunitsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return ProcessUnitStreamRelationStream2ProcessUnit::get<ProcessUnit>(owner->getDatabase(), expr, (ProcessUnitStreamRelationStream2ProcessUnit::Stream == owner->id) && srcExpr);
}
litesql::DataSource<ProcessUnitStreamRelationStream2ProcessUnit::Row> Stream::ProcessunitsHandle::getRows(const litesql::Expr& expr) {
    return ProcessUnitStreamRelationStream2ProcessUnit::getRows(owner->getDatabase(), expr && (ProcessUnitStreamRelationStream2ProcessUnit::Stream == owner->id));
}
const std::string Stream::type__("Stream");
const std::string Stream::table__("Stream_");
const std::string Stream::sequence__("Stream_seq");
const litesql::FieldType Stream::Id("id_","INTEGER",table__);
const litesql::FieldType Stream::Type("type_","TEXT",table__);
const litesql::FieldType Stream::Streamtype("streamtype_","INTEGER",table__);
const litesql::FieldType Stream::Streamindex("streamindex_","INTEGER",table__);
const litesql::FieldType Stream::Duration("duration_","DOUBLE",table__);
void Stream::defaults() {
    id = 0;
    streamtype = 0;
    streamindex = 0;
    duration = 0.0;
}
Stream::Stream(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), streamtype(Streamtype), streamindex(Streamindex), duration(Duration) {
    defaults();
}
Stream::Stream(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), streamtype(Streamtype), streamindex(Streamindex), duration(Duration) {
    defaults();
    size_t size = (rec.size() > 5) ? 5 : rec.size();
    switch(size) {
    case 5: duration = convert<const std::string&, double>(rec[4]);
        duration.setModified(false);
    case 4: streamindex = convert<const std::string&, int>(rec[3]);
        streamindex.setModified(false);
    case 3: streamtype = convert<const std::string&, int>(rec[2]);
        streamtype.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Stream::Stream(const Stream& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), streamtype(obj.streamtype), streamindex(obj.streamindex), duration(obj.duration) {
}
const Stream& Stream::operator=(const Stream& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        streamtype = obj.streamtype;
        streamindex = obj.streamindex;
        duration = obj.duration;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Stream::MediafileHandle Stream::mediafile() {
    return Stream::MediafileHandle(*this);
}
Stream::FormatHandle Stream::format() {
    return Stream::FormatHandle(*this);
}
Stream::ProcessunitsHandle Stream::processunits() {
    return Stream::ProcessunitsHandle(*this);
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
    fields.push_back(streamtype.name());
    values.push_back(streamtype);
    streamtype.setModified(false);
    fields.push_back(streamindex.name());
    values.push_back(streamindex);
    streamindex.setModified(false);
    fields.push_back(duration.name());
    values.push_back(duration);
    duration.setModified(false);
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
    updateField(updates, table__, streamtype);
    updateField(updates, table__, streamindex);
    updateField(updates, table__, duration);
}
void Stream::addIDUpdates(Updates& updates) {
}
void Stream::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Streamtype);
    ftypes.push_back(Streamindex);
    ftypes.push_back(Duration);
}
void Stream::delRecord() {
    deleteFromTable(table__, id);
}
void Stream::delRelations() {
    MediaFileStreamRelationMediaFile2Stream::del(*db, (MediaFileStreamRelationMediaFile2Stream::Stream == id));
    FormatStreamRelationStream2Format::del(*db, (FormatStreamRelationStream2Format::Stream == id));
    ProcessUnitStreamRelationStream2ProcessUnit::del(*db, (ProcessUnitStreamRelationStream2ProcessUnit::Stream == id));
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
    np->streamtype = streamtype;
    np->streamindex = streamindex;
    np->duration = duration;
    np->inDatabase = inDatabase;
    return auto_ptr<Stream>(np);
}
std::ostream & operator<<(std::ostream& os, Stream o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.streamtype.name() << " = " << o.streamtype << std::endl;
    os << o.streamindex.name() << " = " << o.streamindex << std::endl;
    os << o.duration.name() << " = " << o.duration << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType ProcessUnit::Own::Id("id_","INTEGER","ProcessUnit_");
ProcessUnit::StreamHandle::StreamHandle(const ProcessUnit& owner)
         : litesql::RelationHandle<ProcessUnit>(owner) {
}
void ProcessUnit::StreamHandle::link(const Stream& o0) {
    ProcessUnitStreamRelationStream2ProcessUnit::link(owner->getDatabase(), *owner, o0);
}
void ProcessUnit::StreamHandle::unlink(const Stream& o0) {
    ProcessUnitStreamRelationStream2ProcessUnit::unlink(owner->getDatabase(), *owner, o0);
}
void ProcessUnit::StreamHandle::del(const litesql::Expr& expr) {
    ProcessUnitStreamRelationStream2ProcessUnit::del(owner->getDatabase(), expr && ProcessUnitStreamRelationStream2ProcessUnit::ProcessUnit == owner->id);
}
litesql::DataSource<Stream> ProcessUnit::StreamHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return ProcessUnitStreamRelationStream2ProcessUnit::get<Stream>(owner->getDatabase(), expr, (ProcessUnitStreamRelationStream2ProcessUnit::ProcessUnit == owner->id) && srcExpr);
}
litesql::DataSource<ProcessUnitStreamRelationStream2ProcessUnit::Row> ProcessUnit::StreamHandle::getRows(const litesql::Expr& expr) {
    return ProcessUnitStreamRelationStream2ProcessUnit::getRows(owner->getDatabase(), expr && (ProcessUnitStreamRelationStream2ProcessUnit::ProcessUnit == owner->id));
}
const std::string ProcessUnit::type__("ProcessUnit");
const std::string ProcessUnit::table__("ProcessUnit_");
const std::string ProcessUnit::sequence__("ProcessUnit_seq");
const litesql::FieldType ProcessUnit::Id("id_","INTEGER",table__);
const litesql::FieldType ProcessUnit::Type("type_","TEXT",table__);
const litesql::FieldType ProcessUnit::Startts("startts_","DOUBLE",table__);
const litesql::FieldType ProcessUnit::Endts("endts_","DOUBLE",table__);
const litesql::FieldType ProcessUnit::Framecount("framecount_","INTEGER",table__);
const litesql::FieldType ProcessUnit::Send("send_","INTEGER",table__);
const litesql::FieldType ProcessUnit::Recv("recv_","INTEGER",table__);
void ProcessUnit::defaults() {
    id = 0;
    startts = 0.0;
    endts = 0.0;
    framecount = 0;
    send = 0;
    recv = 0;
}
ProcessUnit::ProcessUnit(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), startts(Startts), endts(Endts), framecount(Framecount), send(Send), recv(Recv) {
    defaults();
}
ProcessUnit::ProcessUnit(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), startts(Startts), endts(Endts), framecount(Framecount), send(Send), recv(Recv) {
    defaults();
    size_t size = (rec.size() > 7) ? 7 : rec.size();
    switch(size) {
    case 7: recv = convert<const std::string&, litesql::DateTime>(rec[6]);
        recv.setModified(false);
    case 6: send = convert<const std::string&, litesql::DateTime>(rec[5]);
        send.setModified(false);
    case 5: framecount = convert<const std::string&, int>(rec[4]);
        framecount.setModified(false);
    case 4: endts = convert<const std::string&, double>(rec[3]);
        endts.setModified(false);
    case 3: startts = convert<const std::string&, double>(rec[2]);
        startts.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
ProcessUnit::ProcessUnit(const ProcessUnit& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), startts(obj.startts), endts(obj.endts), framecount(obj.framecount), send(obj.send), recv(obj.recv) {
}
const ProcessUnit& ProcessUnit::operator=(const ProcessUnit& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        startts = obj.startts;
        endts = obj.endts;
        framecount = obj.framecount;
        send = obj.send;
        recv = obj.recv;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
ProcessUnit::StreamHandle ProcessUnit::stream() {
    return ProcessUnit::StreamHandle(*this);
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
    ProcessUnitStreamRelationStream2ProcessUnit::del(*db, (ProcessUnitStreamRelationStream2ProcessUnit::ProcessUnit == id));
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
        res.push_back(Database::SchemaItem("MediaFile_seq","sequence","CREATE SEQUENCE MediaFile_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("MetaData_seq","sequence","CREATE SEQUENCE MetaData_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("Project_seq","sequence","CREATE SEQUENCE Project_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("Job_seq","sequence","CREATE SEQUENCE Job_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("JobLog_seq","sequence","CREATE SEQUENCE JobLog_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("Preset_seq","sequence","CREATE SEQUENCE Preset_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("CodecPreset_seq","sequence","CREATE SEQUENCE CodecPreset_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("Filter_seq","sequence","CREATE SEQUENCE Filter_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("FilterParameter_seq","sequence","CREATE SEQUENCE FilterParameter_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("Format_seq","sequence","CREATE SEQUENCE Format_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("FormatParameter_seq","sequence","CREATE SEQUENCE FormatParameter_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("Stream_seq","sequence","CREATE SEQUENCE Stream_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("ProcessUnit_seq","sequence","CREATE SEQUENCE ProcessUnit_seq START 1 INCREMENT 1"));
    }
    res.push_back(Database::SchemaItem("MediaFile_","table","CREATE TABLE MediaFile_ (id_ " + backend->getRowIDType() + ",type_ TEXT,filepath_ TEXT,filename_ TEXT,filesize_ DOUBLE,streamcount_ INTEGER,containertype_ TEXT,duration_ DOUBLE,starttime_ DOUBLE,bitrate_ INTEGER,filetype_ INTEGER,parent_ INTEGER,created_ INTEGER)"));
    res.push_back(Database::SchemaItem("MetaData_","table","CREATE TABLE MetaData_ (id_ " + backend->getRowIDType() + ",type_ TEXT,metakey_ TEXT,metavalue_ TEXT)"));
    res.push_back(Database::SchemaItem("Project_","table","CREATE TABLE Project_ (id_ " + backend->getRowIDType() + ",type_ TEXT,name_ TEXT,outdirectory_ TEXT,status_ TEXT,created_ INTEGER,started_ INTEGER,completed_ INTEGER)"));
    res.push_back(Database::SchemaItem("Job_","table","CREATE TABLE Job_ (id_ " + backend->getRowIDType() + ",type_ TEXT,created_ INTEGER,begintime_ INTEGER,endtime_ INTEGER,status_ TEXT,infile_ TEXT,outfile_ TEXT,starttime_ DOUBLE,duration_ DOUBLE,progress_ INTEGER)"));
    res.push_back(Database::SchemaItem("JobLog_","table","CREATE TABLE JobLog_ (id_ " + backend->getRowIDType() + ",type_ TEXT,created_ INTEGER,messagetype_ INTEGER,message_ TEXT)"));
    res.push_back(Database::SchemaItem("Preset_","table","CREATE TABLE Preset_ (id_ " + backend->getRowIDType() + ",type_ TEXT,presetname_ TEXT,description_ TEXT,presettype_ INTEGER,created_ INTEGER)"));
    res.push_back(Database::SchemaItem("CodecPreset_","table","CREATE TABLE CodecPreset_ (id_ " + backend->getRowIDType() + ",type_ TEXT,presetname_ TEXT,description_ TEXT,presettype_ INTEGER,created_ INTEGER)"));
    res.push_back(Database::SchemaItem("Filter_","table","CREATE TABLE Filter_ (id_ " + backend->getRowIDType() + ",type_ TEXT,filterid_ TEXT,filtername_ TEXT)"));
    res.push_back(Database::SchemaItem("FilterParameter_","table","CREATE TABLE FilterParameter_ (id_ " + backend->getRowIDType() + ",type_ TEXT,fkey_ TEXT,fval_ TEXT)"));
    res.push_back(Database::SchemaItem("Format_","table","CREATE TABLE Format_ (id_ " + backend->getRowIDType() + ",type_ TEXT,formattype_ INTEGER,formatid_ INTEGER,mimetype_ TEXT)"));
    res.push_back(Database::SchemaItem("FormatParameter_","table","CREATE TABLE FormatParameter_ (id_ " + backend->getRowIDType() + ",type_ TEXT,paramkey_ TEXT,paramvalue_ TEXT)"));
    res.push_back(Database::SchemaItem("Stream_","table","CREATE TABLE Stream_ (id_ " + backend->getRowIDType() + ",type_ TEXT,streamtype_ INTEGER,streamindex_ INTEGER,duration_ DOUBLE)"));
    res.push_back(Database::SchemaItem("ProcessUnit_","table","CREATE TABLE ProcessUnit_ (id_ " + backend->getRowIDType() + ",type_ TEXT,startts_ DOUBLE,endts_ DOUBLE,framecount_ INTEGER,send_ INTEGER,recv_ INTEGER)"));
    res.push_back(Database::SchemaItem("_b861a1aecfe51793ff0c4d3ff4904353","table","CREATE TABLE _b861a1aecfe51793ff0c4d3ff4904353 (MediaFile1 INTEGER,MetaData2 INTEGER)"));
    res.push_back(Database::SchemaItem("_87d2faa689e591de47c1005beca3a5ee","table","CREATE TABLE _87d2faa689e591de47c1005beca3a5ee (MediaFile1 INTEGER,Project2 INTEGER)"));
    res.push_back(Database::SchemaItem("_b94f8c79597ffb2f54a53a65524ba4ab","table","CREATE TABLE _b94f8c79597ffb2f54a53a65524ba4ab (MediaFile1 INTEGER,Project2 INTEGER)"));
    res.push_back(Database::SchemaItem("Job_Project_Project2Job","table","CREATE TABLE Job_Project_Project2Job (Job1 INTEGER,Project2 INTEGER)"));
    res.push_back(Database::SchemaItem("Job_JobLog_Job2JobLog","table","CREATE TABLE Job_JobLog_Job2JobLog (Job1 INTEGER,JobLog2 INTEGER)"));
    res.push_back(Database::SchemaItem("Preset_Project_Project2Preset","table","CREATE TABLE Preset_Project_Project2Preset (Preset1 INTEGER,Project2 INTEGER)"));
    res.push_back(Database::SchemaItem("_fde0f636acb9734e36447df73bd4c50c","table","CREATE TABLE _fde0f636acb9734e36447df73bd4c50c (CodecPreset1 INTEGER,Preset2 INTEGER)"));
    res.push_back(Database::SchemaItem("Filter_Preset_Preset2Filter","table","CREATE TABLE Filter_Preset_Preset2Filter (Filter1 INTEGER,Preset2 INTEGER)"));
    res.push_back(Database::SchemaItem("_0122558569afdaace79061607b81cf8c","table","CREATE TABLE _0122558569afdaace79061607b81cf8c (Filter1 INTEGER,FilterParameter2 INTEGER)"));
    res.push_back(Database::SchemaItem("Format_Preset_Preset2Format","table","CREATE TABLE Format_Preset_Preset2Format (Format1 INTEGER,Preset2 INTEGER)"));
    res.push_back(Database::SchemaItem("_0fd44753f20129838ffe2f1c0dd8acc7","table","CREATE TABLE _0fd44753f20129838ffe2f1c0dd8acc7 (Format1 INTEGER,FormatParameter2 INTEGER)"));
    res.push_back(Database::SchemaItem("_59ad23f924ca1b82be6749fba38a4d77","table","CREATE TABLE _59ad23f924ca1b82be6749fba38a4d77 (MediaFile1 INTEGER,Stream2 INTEGER)"));
    res.push_back(Database::SchemaItem("Format_Stream_Stream2Format","table","CREATE TABLE Format_Stream_Stream2Format (Format1 INTEGER,Stream2 INTEGER)"));
    res.push_back(Database::SchemaItem("_0bd390801550415f76a329f9b7842914","table","CREATE TABLE _0bd390801550415f76a329f9b7842914 (ProcessUnit1 INTEGER,Stream2 INTEGER)"));
    res.push_back(Database::SchemaItem("_53b74cf78d46eabc4efc54aef9c9d5ef","index","CREATE INDEX _53b74cf78d46eabc4efc54aef9c9d5ef ON _b861a1aecfe51793ff0c4d3ff4904353 (MediaFile1)"));
    res.push_back(Database::SchemaItem("_15bcaa6551387d1c0b99d7c331e042a8","index","CREATE INDEX _15bcaa6551387d1c0b99d7c331e042a8 ON _b861a1aecfe51793ff0c4d3ff4904353 (MetaData2)"));
    res.push_back(Database::SchemaItem("_65615f9da36eaf21addb71290f589288","index","CREATE INDEX _65615f9da36eaf21addb71290f589288 ON _b861a1aecfe51793ff0c4d3ff4904353 (MediaFile1,MetaData2)"));
    res.push_back(Database::SchemaItem("_c22651b92af4bf3305bd406d19a8bcd5","index","CREATE INDEX _c22651b92af4bf3305bd406d19a8bcd5 ON _87d2faa689e591de47c1005beca3a5ee (MediaFile1)"));
    res.push_back(Database::SchemaItem("_ccc27cbfe077fee2494771d384e60da7","index","CREATE INDEX _ccc27cbfe077fee2494771d384e60da7 ON _87d2faa689e591de47c1005beca3a5ee (Project2)"));
    res.push_back(Database::SchemaItem("_7e17562f6500f1899b4aedd7bf8dc09d","index","CREATE INDEX _7e17562f6500f1899b4aedd7bf8dc09d ON _87d2faa689e591de47c1005beca3a5ee (MediaFile1,Project2)"));
    res.push_back(Database::SchemaItem("_445b2c6b847dbd4db608c1793a5721c8","index","CREATE INDEX _445b2c6b847dbd4db608c1793a5721c8 ON _b94f8c79597ffb2f54a53a65524ba4ab (MediaFile1)"));
    res.push_back(Database::SchemaItem("_1ebfa6074cd2b44bbce7c3a06cc40619","index","CREATE INDEX _1ebfa6074cd2b44bbce7c3a06cc40619 ON _b94f8c79597ffb2f54a53a65524ba4ab (Project2)"));
    res.push_back(Database::SchemaItem("_c689354eaecad3bcaeaa94721ce09d17","index","CREATE INDEX _c689354eaecad3bcaeaa94721ce09d17 ON _b94f8c79597ffb2f54a53a65524ba4ab (MediaFile1,Project2)"));
    res.push_back(Database::SchemaItem("Job_Project_Project2JobJob1idx","index","CREATE INDEX Job_Project_Project2JobJob1idx ON Job_Project_Project2Job (Job1)"));
    res.push_back(Database::SchemaItem("_75a40c4734882d3c7d4039c9b04920bb","index","CREATE INDEX _75a40c4734882d3c7d4039c9b04920bb ON Job_Project_Project2Job (Project2)"));
    res.push_back(Database::SchemaItem("Job_Project_Project2Job_all_idx","index","CREATE INDEX Job_Project_Project2Job_all_idx ON Job_Project_Project2Job (Job1,Project2)"));
    res.push_back(Database::SchemaItem("Job_JobLog_Job2JobLogJob1idx","index","CREATE INDEX Job_JobLog_Job2JobLogJob1idx ON Job_JobLog_Job2JobLog (Job1)"));
    res.push_back(Database::SchemaItem("Job_JobLog_Job2JobLogJobLog2idx","index","CREATE INDEX Job_JobLog_Job2JobLogJobLog2idx ON Job_JobLog_Job2JobLog (JobLog2)"));
    res.push_back(Database::SchemaItem("Job_JobLog_Job2JobLog_all_idx","index","CREATE INDEX Job_JobLog_Job2JobLog_all_idx ON Job_JobLog_Job2JobLog (Job1,JobLog2)"));
    res.push_back(Database::SchemaItem("_774e7fb5fb45dcf086f1083b36f9c0cd","index","CREATE INDEX _774e7fb5fb45dcf086f1083b36f9c0cd ON Preset_Project_Project2Preset (Preset1)"));
    res.push_back(Database::SchemaItem("_daa7aa7c66250c2adda067ff600be55d","index","CREATE INDEX _daa7aa7c66250c2adda067ff600be55d ON Preset_Project_Project2Preset (Project2)"));
    res.push_back(Database::SchemaItem("_aee065fc0508b8646855eaac2a6c694b","index","CREATE INDEX _aee065fc0508b8646855eaac2a6c694b ON Preset_Project_Project2Preset (Preset1,Project2)"));
    res.push_back(Database::SchemaItem("_5e1dadc2b765c30b567ae6d98115f1e4","index","CREATE INDEX _5e1dadc2b765c30b567ae6d98115f1e4 ON _fde0f636acb9734e36447df73bd4c50c (CodecPreset1)"));
    res.push_back(Database::SchemaItem("_e8245cd9ae78d0fc158d492404c78898","index","CREATE INDEX _e8245cd9ae78d0fc158d492404c78898 ON _fde0f636acb9734e36447df73bd4c50c (Preset2)"));
    res.push_back(Database::SchemaItem("_8c0400e11d1783d1a37cab4e6191e936","index","CREATE INDEX _8c0400e11d1783d1a37cab4e6191e936 ON _fde0f636acb9734e36447df73bd4c50c (CodecPreset1,Preset2)"));
    res.push_back(Database::SchemaItem("_4b4e7d03bfb270e63ffbaedf7bfce5eb","index","CREATE INDEX _4b4e7d03bfb270e63ffbaedf7bfce5eb ON Filter_Preset_Preset2Filter (Filter1)"));
    res.push_back(Database::SchemaItem("_203de15e8cdcc0295ab42437b9381245","index","CREATE INDEX _203de15e8cdcc0295ab42437b9381245 ON Filter_Preset_Preset2Filter (Preset2)"));
    res.push_back(Database::SchemaItem("_8063108ef98e73d0af42471584d69bf3","index","CREATE INDEX _8063108ef98e73d0af42471584d69bf3 ON Filter_Preset_Preset2Filter (Filter1,Preset2)"));
    res.push_back(Database::SchemaItem("_130771f9c2a0ff36ca5f2e78bc08af8b","index","CREATE INDEX _130771f9c2a0ff36ca5f2e78bc08af8b ON _0122558569afdaace79061607b81cf8c (Filter1)"));
    res.push_back(Database::SchemaItem("_88fa56b3f94401d004daccd34fb1fdf9","index","CREATE INDEX _88fa56b3f94401d004daccd34fb1fdf9 ON _0122558569afdaace79061607b81cf8c (FilterParameter2)"));
    res.push_back(Database::SchemaItem("_322bd285b6b3e4f16b182c5e9174b4f4","index","CREATE INDEX _322bd285b6b3e4f16b182c5e9174b4f4 ON _0122558569afdaace79061607b81cf8c (Filter1,FilterParameter2)"));
    res.push_back(Database::SchemaItem("_3b0e4e4993af7298970b228f64d27f7d","index","CREATE INDEX _3b0e4e4993af7298970b228f64d27f7d ON Format_Preset_Preset2Format (Format1)"));
    res.push_back(Database::SchemaItem("_f4bcc84493831958f8e4cd23dd8cd9b6","index","CREATE INDEX _f4bcc84493831958f8e4cd23dd8cd9b6 ON Format_Preset_Preset2Format (Preset2)"));
    res.push_back(Database::SchemaItem("_9f02f1568b9fc7430df24ab5ddb58070","index","CREATE INDEX _9f02f1568b9fc7430df24ab5ddb58070 ON Format_Preset_Preset2Format (Format1,Preset2)"));
    res.push_back(Database::SchemaItem("_249e07950834178178738ae525982937","index","CREATE INDEX _249e07950834178178738ae525982937 ON _0fd44753f20129838ffe2f1c0dd8acc7 (Format1)"));
    res.push_back(Database::SchemaItem("_70adb571e0baeb099f12821ae70921da","index","CREATE INDEX _70adb571e0baeb099f12821ae70921da ON _0fd44753f20129838ffe2f1c0dd8acc7 (FormatParameter2)"));
    res.push_back(Database::SchemaItem("_b162ee9e5badb912a1e14788dd884b4f","index","CREATE INDEX _b162ee9e5badb912a1e14788dd884b4f ON _0fd44753f20129838ffe2f1c0dd8acc7 (Format1,FormatParameter2)"));
    res.push_back(Database::SchemaItem("_c83d673fdad27db9e6c3ade97b8479d2","index","CREATE INDEX _c83d673fdad27db9e6c3ade97b8479d2 ON _59ad23f924ca1b82be6749fba38a4d77 (MediaFile1)"));
    res.push_back(Database::SchemaItem("_84b435710c41d95435919946cb0cc910","index","CREATE INDEX _84b435710c41d95435919946cb0cc910 ON _59ad23f924ca1b82be6749fba38a4d77 (Stream2)"));
    res.push_back(Database::SchemaItem("_f2c9133864f1c7fc9cf599334b85bdb7","index","CREATE INDEX _f2c9133864f1c7fc9cf599334b85bdb7 ON _59ad23f924ca1b82be6749fba38a4d77 (MediaFile1,Stream2)"));
    res.push_back(Database::SchemaItem("_21af070e9e27006df13448fe4c802b57","index","CREATE INDEX _21af070e9e27006df13448fe4c802b57 ON Format_Stream_Stream2Format (Format1)"));
    res.push_back(Database::SchemaItem("_17284cf25900bbb56250c7422a00954b","index","CREATE INDEX _17284cf25900bbb56250c7422a00954b ON Format_Stream_Stream2Format (Stream2)"));
    res.push_back(Database::SchemaItem("_ec446fa6ca0850f111bd624bc7806a0f","index","CREATE INDEX _ec446fa6ca0850f111bd624bc7806a0f ON Format_Stream_Stream2Format (Format1,Stream2)"));
    res.push_back(Database::SchemaItem("_8a58edbf12f8bbc0a53ce2547c428700","index","CREATE INDEX _8a58edbf12f8bbc0a53ce2547c428700 ON _0bd390801550415f76a329f9b7842914 (ProcessUnit1)"));
    res.push_back(Database::SchemaItem("_c7cc94e913dc012495d70cb8854a3b77","index","CREATE INDEX _c7cc94e913dc012495d70cb8854a3b77 ON _0bd390801550415f76a329f9b7842914 (Stream2)"));
    res.push_back(Database::SchemaItem("_4f65949e8c6b8f2841dab58a43cc5bea","index","CREATE INDEX _4f65949e8c6b8f2841dab58a43cc5bea ON _0bd390801550415f76a329f9b7842914 (ProcessUnit1,Stream2)"));
    return res;
}
void HiveDb::initialize() {
    static bool initialized = false;
    if (initialized)
        return;
    initialized = true;
}
}
