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
const litesql::FieldType FilterFilterParameterRelation::Filter("Filter1_",A_field_type_integer,table__);
const litesql::FieldType FilterFilterParameterRelation::FilterParameter("FilterParameter2_",A_field_type_integer,table__);
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
FilterMediaFileRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : mediaFile(FilterMediaFileRelation::MediaFile), filter(FilterMediaFileRelation::Filter) {
    switch(rec.size()) {
    case 2:
        mediaFile = rec[1];
    case 1:
        filter = rec[0];
    }
}
const std::string FilterMediaFileRelation::table__("Filter_MediaFile_");
const litesql::FieldType FilterMediaFileRelation::Filter("Filter1_",A_field_type_integer,table__);
const litesql::FieldType FilterMediaFileRelation::MediaFile("MediaFile2_",A_field_type_integer,table__);
void FilterMediaFileRelation::link(const litesql::Database& db, const db::Filter& o0, const db::MediaFile& o1) {
    Record values;
    Split fields;
    fields.push_back(Filter.name());
    values.push_back(o0.id);
    fields.push_back(MediaFile.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void FilterMediaFileRelation::unlink(const litesql::Database& db, const db::Filter& o0, const db::MediaFile& o1) {
    db.delete_(table__, (Filter == o0.id && MediaFile == o1.id));
}
void FilterMediaFileRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<FilterMediaFileRelation::Row> FilterMediaFileRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Filter.fullName());
    sel.result(MediaFile.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<FilterMediaFileRelation::Row>(db, sel);
}
template <> litesql::DataSource<db::Filter> FilterMediaFileRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Filter.fullName());
    sel.where(srcExpr);
    return DataSource<db::Filter>(db, db::Filter::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::MediaFile> FilterMediaFileRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(MediaFile.fullName());
    sel.where(srcExpr);
    return DataSource<db::MediaFile>(db, db::MediaFile::Id.in(sel) && expr);
}
FilterProjectRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : project(FilterProjectRelation::Project), filter(FilterProjectRelation::Filter) {
    switch(rec.size()) {
    case 2:
        project = rec[1];
    case 1:
        filter = rec[0];
    }
}
const std::string FilterProjectRelation::table__("Filter_Project_");
const litesql::FieldType FilterProjectRelation::Filter("Filter1_",A_field_type_integer,table__);
const litesql::FieldType FilterProjectRelation::Project("Project2_",A_field_type_integer,table__);
void FilterProjectRelation::link(const litesql::Database& db, const db::Filter& o0, const db::Project& o1) {
    Record values;
    Split fields;
    fields.push_back(Filter.name());
    values.push_back(o0.id);
    fields.push_back(Project.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void FilterProjectRelation::unlink(const litesql::Database& db, const db::Filter& o0, const db::Project& o1) {
    db.delete_(table__, (Filter == o0.id && Project == o1.id));
}
void FilterProjectRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<FilterProjectRelation::Row> FilterProjectRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Filter.fullName());
    sel.result(Project.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<FilterProjectRelation::Row>(db, sel);
}
template <> litesql::DataSource<db::Filter> FilterProjectRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Filter.fullName());
    sel.where(srcExpr);
    return DataSource<db::Filter>(db, db::Filter::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::Project> FilterProjectRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Project.fullName());
    sel.where(srcExpr);
    return DataSource<db::Project>(db, db::Project::Id.in(sel) && expr);
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
const litesql::FieldType MediaFileProjectRelation::MediaFile("MediaFile1_",A_field_type_integer,table__);
const litesql::FieldType MediaFileProjectRelation::Project("Project2_",A_field_type_integer,table__);
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
PresetProjectRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : project(PresetProjectRelation::Project), preset(PresetProjectRelation::Preset) {
    switch(rec.size()) {
    case 2:
        project = rec[1];
    case 1:
        preset = rec[0];
    }
}
const std::string PresetProjectRelation::table__("Preset_Project_");
const litesql::FieldType PresetProjectRelation::Preset("Preset1_",A_field_type_integer,table__);
const litesql::FieldType PresetProjectRelation::Project("Project2_",A_field_type_integer,table__);
void PresetProjectRelation::link(const litesql::Database& db, const db::Preset& o0, const db::Project& o1) {
    Record values;
    Split fields;
    fields.push_back(Preset.name());
    values.push_back(o0.id);
    fields.push_back(Project.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void PresetProjectRelation::unlink(const litesql::Database& db, const db::Preset& o0, const db::Project& o1) {
    db.delete_(table__, (Preset == o0.id && Project == o1.id));
}
void PresetProjectRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<PresetProjectRelation::Row> PresetProjectRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Preset.fullName());
    sel.result(Project.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<PresetProjectRelation::Row>(db, sel);
}
template <> litesql::DataSource<db::Preset> PresetProjectRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Preset.fullName());
    sel.where(srcExpr);
    return DataSource<db::Preset>(db, db::Preset::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::Project> PresetProjectRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Project.fullName());
    sel.where(srcExpr);
    return DataSource<db::Project>(db, db::Project::Id.in(sel) && expr);
}
PresetUserRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : user(PresetUserRelation::User), preset(PresetUserRelation::Preset) {
    switch(rec.size()) {
    case 2:
        user = rec[1];
    case 1:
        preset = rec[0];
    }
}
const std::string PresetUserRelation::table__("Preset_User_");
const litesql::FieldType PresetUserRelation::Preset("Preset1_",A_field_type_integer,table__);
const litesql::FieldType PresetUserRelation::User("User2_",A_field_type_integer,table__);
void PresetUserRelation::link(const litesql::Database& db, const db::Preset& o0, const db::User& o1) {
    Record values;
    Split fields;
    fields.push_back(Preset.name());
    values.push_back(o0.id);
    fields.push_back(User.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void PresetUserRelation::unlink(const litesql::Database& db, const db::Preset& o0, const db::User& o1) {
    db.delete_(table__, (Preset == o0.id && User == o1.id));
}
void PresetUserRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<PresetUserRelation::Row> PresetUserRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Preset.fullName());
    sel.result(User.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<PresetUserRelation::Row>(db, sel);
}
template <> litesql::DataSource<db::Preset> PresetUserRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Preset.fullName());
    sel.where(srcExpr);
    return DataSource<db::Preset>(db, db::Preset::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::User> PresetUserRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(User.fullName());
    sel.where(srcExpr);
    return DataSource<db::User>(db, db::User::Id.in(sel) && expr);
}
FilterProfileRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : profile(FilterProfileRelation::Profile), filter(FilterProfileRelation::Filter) {
    switch(rec.size()) {
    case 2:
        profile = rec[1];
    case 1:
        filter = rec[0];
    }
}
const std::string FilterProfileRelation::table__("Filter_Profile_");
const litesql::FieldType FilterProfileRelation::Filter("Filter1_",A_field_type_integer,table__);
const litesql::FieldType FilterProfileRelation::Profile("Profile2_",A_field_type_integer,table__);
void FilterProfileRelation::link(const litesql::Database& db, const db::Filter& o0, const db::Profile& o1) {
    Record values;
    Split fields;
    fields.push_back(Filter.name());
    values.push_back(o0.id);
    fields.push_back(Profile.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void FilterProfileRelation::unlink(const litesql::Database& db, const db::Filter& o0, const db::Profile& o1) {
    db.delete_(table__, (Filter == o0.id && Profile == o1.id));
}
void FilterProfileRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<FilterProfileRelation::Row> FilterProfileRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Filter.fullName());
    sel.result(Profile.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<FilterProfileRelation::Row>(db, sel);
}
template <> litesql::DataSource<db::Filter> FilterProfileRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Filter.fullName());
    sel.where(srcExpr);
    return DataSource<db::Filter>(db, db::Filter::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::Profile> FilterProfileRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Profile.fullName());
    sel.where(srcExpr);
    return DataSource<db::Profile>(db, db::Profile::Id.in(sel) && expr);
}
ProfileGroup2Profile::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : profileGroup(ProfileGroup2Profile::ProfileGroup), profile(ProfileGroup2Profile::Profile) {
    switch(rec.size()) {
    case 2:
        profileGroup = rec[1];
    case 1:
        profile = rec[0];
    }
}
const std::string ProfileGroup2Profile::table__("O60a643d384d1f6c6ddeba5bb8ac0f");
const litesql::FieldType ProfileGroup2Profile::Profile("Profile1_",A_field_type_integer,table__);
const litesql::FieldType ProfileGroup2Profile::ProfileGroup("ProfileGroup2_",A_field_type_integer,table__);
void ProfileGroup2Profile::link(const litesql::Database& db, const db::Profile& o0, const db::ProfileGroup& o1) {
    Record values;
    Split fields;
    fields.push_back(Profile.name());
    values.push_back(o0.id);
    fields.push_back(ProfileGroup.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void ProfileGroup2Profile::unlink(const litesql::Database& db, const db::Profile& o0, const db::ProfileGroup& o1) {
    db.delete_(table__, (Profile == o0.id && ProfileGroup == o1.id));
}
void ProfileGroup2Profile::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<ProfileGroup2Profile::Row> ProfileGroup2Profile::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Profile.fullName());
    sel.result(ProfileGroup.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<ProfileGroup2Profile::Row>(db, sel);
}
template <> litesql::DataSource<db::Profile> ProfileGroup2Profile::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Profile.fullName());
    sel.where(srcExpr);
    return DataSource<db::Profile>(db, db::Profile::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::ProfileGroup> ProfileGroup2Profile::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(ProfileGroup.fullName());
    sel.where(srcExpr);
    return DataSource<db::ProfileGroup>(db, db::ProfileGroup::Id.in(sel) && expr);
}
ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : profileGroup2(ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::ProfileGroup2), profileGroup1(ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::ProfileGroup1) {
    switch(rec.size()) {
    case 2:
        profileGroup2 = rec[1];
    case 1:
        profileGroup1 = rec[0];
    }
}
const std::string ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::table__("O2d57c481daf84ed6d04cd9e705469");
const litesql::FieldType ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::ProfileGroup1("ProfileGroup1_",A_field_type_integer,table__);
const litesql::FieldType ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::ProfileGroup2("ProfileGroup2_",A_field_type_integer,table__);
void ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::link(const litesql::Database& db, const db::ProfileGroup& o0, const db::ProfileGroup& o1) {
    Record values;
    Split fields;
    fields.push_back(ProfileGroup1.name());
    values.push_back(o0.id);
    fields.push_back(ProfileGroup2.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::unlink(const litesql::Database& db, const db::ProfileGroup& o0, const db::ProfileGroup& o1) {
    db.delete_(table__, (ProfileGroup1 == o0.id && ProfileGroup2 == o1.id));
}
void ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::Row> ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(ProfileGroup1.fullName());
    sel.result(ProfileGroup2.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::Row>(db, sel);
}
litesql::DataSource<db::ProfileGroup> ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::getProfileGroup1(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(ProfileGroup1.fullName());
    sel.where(srcExpr);
    return DataSource<db::ProfileGroup>(db, db::ProfileGroup::Id.in(sel) && expr);
}
litesql::DataSource<db::ProfileGroup> ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::getProfileGroup2(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(ProfileGroup2.fullName());
    sel.where(srcExpr);
    return DataSource<db::ProfileGroup>(db, db::ProfileGroup::Id.in(sel) && expr);
}
ProfileProfileParameterRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : profileParameter(ProfileProfileParameterRelation::ProfileParameter), profile(ProfileProfileParameterRelation::Profile) {
    switch(rec.size()) {
    case 2:
        profileParameter = rec[1];
    case 1:
        profile = rec[0];
    }
}
const std::string ProfileProfileParameterRelation::table__("Profile_ProfileParameter_");
const litesql::FieldType ProfileProfileParameterRelation::Profile("Profile1_",A_field_type_integer,table__);
const litesql::FieldType ProfileProfileParameterRelation::ProfileParameter("ProfileParameter2_",A_field_type_integer,table__);
void ProfileProfileParameterRelation::link(const litesql::Database& db, const db::Profile& o0, const db::ProfileParameter& o1) {
    Record values;
    Split fields;
    fields.push_back(Profile.name());
    values.push_back(o0.id);
    fields.push_back(ProfileParameter.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void ProfileProfileParameterRelation::unlink(const litesql::Database& db, const db::Profile& o0, const db::ProfileParameter& o1) {
    db.delete_(table__, (Profile == o0.id && ProfileParameter == o1.id));
}
void ProfileProfileParameterRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<ProfileProfileParameterRelation::Row> ProfileProfileParameterRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Profile.fullName());
    sel.result(ProfileParameter.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<ProfileProfileParameterRelation::Row>(db, sel);
}
template <> litesql::DataSource<db::Profile> ProfileProfileParameterRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Profile.fullName());
    sel.where(srcExpr);
    return DataSource<db::Profile>(db, db::Profile::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::ProfileParameter> ProfileProfileParameterRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(ProfileParameter.fullName());
    sel.where(srcExpr);
    return DataSource<db::ProfileParameter>(db, db::ProfileParameter::Id.in(sel) && expr);
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
const litesql::FieldType ProfileProjectRelation::Profile("Profile1_",A_field_type_integer,table__);
const litesql::FieldType ProfileProjectRelation::Project("Project2_",A_field_type_integer,table__);
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
StreamStreamParameterRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : streamParameter(StreamStreamParameterRelation::StreamParameter), stream(StreamStreamParameterRelation::Stream) {
    switch(rec.size()) {
    case 2:
        streamParameter = rec[1];
    case 1:
        stream = rec[0];
    }
}
const std::string StreamStreamParameterRelation::table__("Stream_StreamParameter_");
const litesql::FieldType StreamStreamParameterRelation::Stream("Stream1_",A_field_type_integer,table__);
const litesql::FieldType StreamStreamParameterRelation::StreamParameter("StreamParameter2_",A_field_type_integer,table__);
void StreamStreamParameterRelation::link(const litesql::Database& db, const db::Stream& o0, const db::StreamParameter& o1) {
    Record values;
    Split fields;
    fields.push_back(Stream.name());
    values.push_back(o0.id);
    fields.push_back(StreamParameter.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void StreamStreamParameterRelation::unlink(const litesql::Database& db, const db::Stream& o0, const db::StreamParameter& o1) {
    db.delete_(table__, (Stream == o0.id && StreamParameter == o1.id));
}
void StreamStreamParameterRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<StreamStreamParameterRelation::Row> StreamStreamParameterRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Stream.fullName());
    sel.result(StreamParameter.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<StreamStreamParameterRelation::Row>(db, sel);
}
template <> litesql::DataSource<db::Stream> StreamStreamParameterRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Stream.fullName());
    sel.where(srcExpr);
    return DataSource<db::Stream>(db, db::Stream::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::StreamParameter> StreamStreamParameterRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(StreamParameter.fullName());
    sel.where(srcExpr);
    return DataSource<db::StreamParameter>(db, db::StreamParameter::Id.in(sel) && expr);
}
FilterStreamRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : stream(FilterStreamRelation::Stream), filter(FilterStreamRelation::Filter) {
    switch(rec.size()) {
    case 2:
        stream = rec[1];
    case 1:
        filter = rec[0];
    }
}
const std::string FilterStreamRelation::table__("Filter_Stream_");
const litesql::FieldType FilterStreamRelation::Filter("Filter1_",A_field_type_integer,table__);
const litesql::FieldType FilterStreamRelation::Stream("Stream2_",A_field_type_integer,table__);
void FilterStreamRelation::link(const litesql::Database& db, const db::Filter& o0, const db::Stream& o1) {
    Record values;
    Split fields;
    fields.push_back(Filter.name());
    values.push_back(o0.id);
    fields.push_back(Stream.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void FilterStreamRelation::unlink(const litesql::Database& db, const db::Filter& o0, const db::Stream& o1) {
    db.delete_(table__, (Filter == o0.id && Stream == o1.id));
}
void FilterStreamRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<FilterStreamRelation::Row> FilterStreamRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Filter.fullName());
    sel.result(Stream.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<FilterStreamRelation::Row>(db, sel);
}
template <> litesql::DataSource<db::Filter> FilterStreamRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Filter.fullName());
    sel.where(srcExpr);
    return DataSource<db::Filter>(db, db::Filter::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::Stream> FilterStreamRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Stream.fullName());
    sel.where(srcExpr);
    return DataSource<db::Stream>(db, db::Stream::Id.in(sel) && expr);
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
const litesql::FieldType MediaFileStreamRelation::MediaFile("MediaFile1_",A_field_type_integer,table__);
const litesql::FieldType MediaFileStreamRelation::Stream("Stream2_",A_field_type_integer,table__);
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
CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : codecPresetParameter(CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::CodecPresetParameter), codecPreset(CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::CodecPreset) {
    switch(rec.size()) {
    case 2:
        codecPresetParameter = rec[1];
    case 1:
        codecPreset = rec[0];
    }
}
const std::string CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::table__("O165bce89be0b4f99d8ddeba7a26a2");
const litesql::FieldType CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::CodecPreset("CodecPreset1_",A_field_type_integer,table__);
const litesql::FieldType CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::CodecPresetParameter("CodecPresetParameter2_",A_field_type_integer,table__);
void CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::link(const litesql::Database& db, const db::CodecPreset& o0, const db::CodecPresetParameter& o1) {
    Record values;
    Split fields;
    fields.push_back(CodecPreset.name());
    values.push_back(o0.id);
    fields.push_back(CodecPresetParameter.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::unlink(const litesql::Database& db, const db::CodecPreset& o0, const db::CodecPresetParameter& o1) {
    db.delete_(table__, (CodecPreset == o0.id && CodecPresetParameter == o1.id));
}
void CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::Row> CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(CodecPreset.fullName());
    sel.result(CodecPresetParameter.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::Row>(db, sel);
}
template <> litesql::DataSource<db::CodecPreset> CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(CodecPreset.fullName());
    sel.where(srcExpr);
    return DataSource<db::CodecPreset>(db, db::CodecPreset::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::CodecPresetParameter> CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(CodecPresetParameter.fullName());
    sel.where(srcExpr);
    return DataSource<db::CodecPresetParameter>(db, db::CodecPresetParameter::Id.in(sel) && expr);
}
CodecPresetProfileRelationVideoCodecPreset2Profile::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : profile(CodecPresetProfileRelationVideoCodecPreset2Profile::Profile), codecPreset(CodecPresetProfileRelationVideoCodecPreset2Profile::CodecPreset) {
    switch(rec.size()) {
    case 2:
        profile = rec[1];
    case 1:
        codecPreset = rec[0];
    }
}
const std::string CodecPresetProfileRelationVideoCodecPreset2Profile::table__("Ob477e426317c3764439827c70cd95");
const litesql::FieldType CodecPresetProfileRelationVideoCodecPreset2Profile::CodecPreset("CodecPreset1_",A_field_type_integer,table__);
const litesql::FieldType CodecPresetProfileRelationVideoCodecPreset2Profile::Profile("Profile2_",A_field_type_integer,table__);
void CodecPresetProfileRelationVideoCodecPreset2Profile::link(const litesql::Database& db, const db::CodecPreset& o0, const db::Profile& o1) {
    Record values;
    Split fields;
    fields.push_back(CodecPreset.name());
    values.push_back(o0.id);
    fields.push_back(Profile.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void CodecPresetProfileRelationVideoCodecPreset2Profile::unlink(const litesql::Database& db, const db::CodecPreset& o0, const db::Profile& o1) {
    db.delete_(table__, (CodecPreset == o0.id && Profile == o1.id));
}
void CodecPresetProfileRelationVideoCodecPreset2Profile::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<CodecPresetProfileRelationVideoCodecPreset2Profile::Row> CodecPresetProfileRelationVideoCodecPreset2Profile::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(CodecPreset.fullName());
    sel.result(Profile.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<CodecPresetProfileRelationVideoCodecPreset2Profile::Row>(db, sel);
}
template <> litesql::DataSource<db::CodecPreset> CodecPresetProfileRelationVideoCodecPreset2Profile::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(CodecPreset.fullName());
    sel.where(srcExpr);
    return DataSource<db::CodecPreset>(db, db::CodecPreset::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::Profile> CodecPresetProfileRelationVideoCodecPreset2Profile::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Profile.fullName());
    sel.where(srcExpr);
    return DataSource<db::Profile>(db, db::Profile::Id.in(sel) && expr);
}
CodecPresetProfileRelationAudioCodecPreset2Profile::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : profile(CodecPresetProfileRelationAudioCodecPreset2Profile::Profile), codecPreset(CodecPresetProfileRelationAudioCodecPreset2Profile::CodecPreset) {
    switch(rec.size()) {
    case 2:
        profile = rec[1];
    case 1:
        codecPreset = rec[0];
    }
}
const std::string CodecPresetProfileRelationAudioCodecPreset2Profile::table__("Oc47426250800c92cff81a427efb64");
const litesql::FieldType CodecPresetProfileRelationAudioCodecPreset2Profile::CodecPreset("CodecPreset1_",A_field_type_integer,table__);
const litesql::FieldType CodecPresetProfileRelationAudioCodecPreset2Profile::Profile("Profile2_",A_field_type_integer,table__);
void CodecPresetProfileRelationAudioCodecPreset2Profile::link(const litesql::Database& db, const db::CodecPreset& o0, const db::Profile& o1) {
    Record values;
    Split fields;
    fields.push_back(CodecPreset.name());
    values.push_back(o0.id);
    fields.push_back(Profile.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void CodecPresetProfileRelationAudioCodecPreset2Profile::unlink(const litesql::Database& db, const db::CodecPreset& o0, const db::Profile& o1) {
    db.delete_(table__, (CodecPreset == o0.id && Profile == o1.id));
}
void CodecPresetProfileRelationAudioCodecPreset2Profile::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<CodecPresetProfileRelationAudioCodecPreset2Profile::Row> CodecPresetProfileRelationAudioCodecPreset2Profile::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(CodecPreset.fullName());
    sel.result(Profile.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<CodecPresetProfileRelationAudioCodecPreset2Profile::Row>(db, sel);
}
template <> litesql::DataSource<db::CodecPreset> CodecPresetProfileRelationAudioCodecPreset2Profile::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(CodecPreset.fullName());
    sel.where(srcExpr);
    return DataSource<db::CodecPreset>(db, db::CodecPreset::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::Profile> CodecPresetProfileRelationAudioCodecPreset2Profile::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Profile.fullName());
    sel.where(srcExpr);
    return DataSource<db::Profile>(db, db::Profile::Id.in(sel) && expr);
}
JobTaskRelationJobTask::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : task(JobTaskRelationJobTask::Task), job(JobTaskRelationJobTask::Job) {
    switch(rec.size()) {
    case 2:
        task = rec[1];
    case 1:
        job = rec[0];
    }
}
const std::string JobTaskRelationJobTask::table__("Job_Task_JobTask");
const litesql::FieldType JobTaskRelationJobTask::Job("Job1_",A_field_type_integer,table__);
const litesql::FieldType JobTaskRelationJobTask::Task("Task2_",A_field_type_integer,table__);
void JobTaskRelationJobTask::link(const litesql::Database& db, const db::Job& o0, const db::Task& o1) {
    Record values;
    Split fields;
    fields.push_back(Job.name());
    values.push_back(o0.id);
    fields.push_back(Task.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void JobTaskRelationJobTask::unlink(const litesql::Database& db, const db::Job& o0, const db::Task& o1) {
    db.delete_(table__, (Job == o0.id && Task == o1.id));
}
void JobTaskRelationJobTask::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<JobTaskRelationJobTask::Row> JobTaskRelationJobTask::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Job.fullName());
    sel.result(Task.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<JobTaskRelationJobTask::Row>(db, sel);
}
template <> litesql::DataSource<db::Job> JobTaskRelationJobTask::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Job.fullName());
    sel.where(srcExpr);
    return DataSource<db::Job>(db, db::Job::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::Task> JobTaskRelationJobTask::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Task.fullName());
    sel.where(srcExpr);
    return DataSource<db::Task>(db, db::Task::Id.in(sel) && expr);
}
GraphTaskRelationGraphTask::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : task(GraphTaskRelationGraphTask::Task), graph(GraphTaskRelationGraphTask::Graph) {
    switch(rec.size()) {
    case 2:
        task = rec[1];
    case 1:
        graph = rec[0];
    }
}
const std::string GraphTaskRelationGraphTask::table__("Graph_Task_GraphTask");
const litesql::FieldType GraphTaskRelationGraphTask::Graph("Graph1_",A_field_type_integer,table__);
const litesql::FieldType GraphTaskRelationGraphTask::Task("Task2_",A_field_type_integer,table__);
void GraphTaskRelationGraphTask::link(const litesql::Database& db, const db::Graph& o0, const db::Task& o1) {
    Record values;
    Split fields;
    fields.push_back(Graph.name());
    values.push_back(o0.id);
    fields.push_back(Task.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void GraphTaskRelationGraphTask::unlink(const litesql::Database& db, const db::Graph& o0, const db::Task& o1) {
    db.delete_(table__, (Graph == o0.id && Task == o1.id));
}
void GraphTaskRelationGraphTask::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<GraphTaskRelationGraphTask::Row> GraphTaskRelationGraphTask::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Graph.fullName());
    sel.result(Task.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<GraphTaskRelationGraphTask::Row>(db, sel);
}
template <> litesql::DataSource<db::Graph> GraphTaskRelationGraphTask::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Graph.fullName());
    sel.where(srcExpr);
    return DataSource<db::Graph>(db, db::Graph::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::Task> GraphTaskRelationGraphTask::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Task.fullName());
    sel.where(srcExpr);
    return DataSource<db::Task>(db, db::Task::Id.in(sel) && expr);
}
JobJobLogRelationJobJobLog::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : jobLog(JobJobLogRelationJobJobLog::JobLog), job(JobJobLogRelationJobJobLog::Job) {
    switch(rec.size()) {
    case 2:
        jobLog = rec[1];
    case 1:
        job = rec[0];
    }
}
const std::string JobJobLogRelationJobJobLog::table__("Job_JobLog_JobJobLog");
const litesql::FieldType JobJobLogRelationJobJobLog::Job("Job1_",A_field_type_integer,table__);
const litesql::FieldType JobJobLogRelationJobJobLog::JobLog("JobLog2_",A_field_type_integer,table__);
void JobJobLogRelationJobJobLog::link(const litesql::Database& db, const db::Job& o0, const db::JobLog& o1) {
    Record values;
    Split fields;
    fields.push_back(Job.name());
    values.push_back(o0.id);
    fields.push_back(JobLog.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void JobJobLogRelationJobJobLog::unlink(const litesql::Database& db, const db::Job& o0, const db::JobLog& o1) {
    db.delete_(table__, (Job == o0.id && JobLog == o1.id));
}
void JobJobLogRelationJobJobLog::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<JobJobLogRelationJobJobLog::Row> JobJobLogRelationJobJobLog::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Job.fullName());
    sel.result(JobLog.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<JobJobLogRelationJobJobLog::Row>(db, sel);
}
template <> litesql::DataSource<db::Job> JobJobLogRelationJobJobLog::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Job.fullName());
    sel.where(srcExpr);
    return DataSource<db::Job>(db, db::Job::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::JobLog> JobJobLogRelationJobJobLog::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(JobLog.fullName());
    sel.where(srcExpr);
    return DataSource<db::JobLog>(db, db::JobLog::Id.in(sel) && expr);
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
const litesql::FieldType JobMediaFileRelationJobInFile::Job("Job1_",A_field_type_integer,table__);
const litesql::FieldType JobMediaFileRelationJobInFile::MediaFile("MediaFile2_",A_field_type_integer,table__);
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
const litesql::FieldType JobMediaFileRelationJobOutFile::Job("Job1_",A_field_type_integer,table__);
const litesql::FieldType JobMediaFileRelationJobOutFile::MediaFile("MediaFile2_",A_field_type_integer,table__);
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
JobPresetRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : preset(JobPresetRelation::Preset), job(JobPresetRelation::Job) {
    switch(rec.size()) {
    case 2:
        preset = rec[1];
    case 1:
        job = rec[0];
    }
}
const std::string JobPresetRelation::table__("Job_Preset_");
const litesql::FieldType JobPresetRelation::Job("Job1_",A_field_type_integer,table__);
const litesql::FieldType JobPresetRelation::Preset("Preset2_",A_field_type_integer,table__);
void JobPresetRelation::link(const litesql::Database& db, const db::Job& o0, const db::Preset& o1) {
    Record values;
    Split fields;
    fields.push_back(Job.name());
    values.push_back(o0.id);
    fields.push_back(Preset.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void JobPresetRelation::unlink(const litesql::Database& db, const db::Job& o0, const db::Preset& o1) {
    db.delete_(table__, (Job == o0.id && Preset == o1.id));
}
void JobPresetRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<JobPresetRelation::Row> JobPresetRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Job.fullName());
    sel.result(Preset.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<JobPresetRelation::Row>(db, sel);
}
template <> litesql::DataSource<db::Job> JobPresetRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Job.fullName());
    sel.where(srcExpr);
    return DataSource<db::Job>(db, db::Job::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::Preset> JobPresetRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Preset.fullName());
    sel.where(srcExpr);
    return DataSource<db::Preset>(db, db::Preset::Id.in(sel) && expr);
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
const litesql::FieldType JobJobDetailRelationJobJobDetail::Job("Job1_",A_field_type_integer,table__);
const litesql::FieldType JobJobDetailRelationJobJobDetail::JobDetail("JobDetail2_",A_field_type_integer,table__);
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
const litesql::FieldType JobDetailStreamRelationJobOutStream::JobDetail("JobDetail1_",A_field_type_integer,table__);
const litesql::FieldType JobDetailStreamRelationJobOutStream::Stream("Stream2_",A_field_type_integer,table__);
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
const litesql::FieldType JobDetailStreamRelationJobInStream::JobDetail("JobDetail1_",A_field_type_integer,table__);
const litesql::FieldType JobDetailStreamRelationJobInStream::Stream("Stream2_",A_field_type_integer,table__);
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
JobProcessUnitRelationProcessUnitJob::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : processUnit(JobProcessUnitRelationProcessUnitJob::ProcessUnit), job(JobProcessUnitRelationProcessUnitJob::Job) {
    switch(rec.size()) {
    case 2:
        processUnit = rec[1];
    case 1:
        job = rec[0];
    }
}
const std::string JobProcessUnitRelationProcessUnitJob::table__("Job_ProcessUnit_ProcessUnitJob");
const litesql::FieldType JobProcessUnitRelationProcessUnitJob::Job("Job1_",A_field_type_integer,table__);
const litesql::FieldType JobProcessUnitRelationProcessUnitJob::ProcessUnit("ProcessUnit2_",A_field_type_integer,table__);
void JobProcessUnitRelationProcessUnitJob::link(const litesql::Database& db, const db::Job& o0, const db::ProcessUnit& o1) {
    Record values;
    Split fields;
    fields.push_back(Job.name());
    values.push_back(o0.id);
    fields.push_back(ProcessUnit.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void JobProcessUnitRelationProcessUnitJob::unlink(const litesql::Database& db, const db::Job& o0, const db::ProcessUnit& o1) {
    db.delete_(table__, (Job == o0.id && ProcessUnit == o1.id));
}
void JobProcessUnitRelationProcessUnitJob::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<JobProcessUnitRelationProcessUnitJob::Row> JobProcessUnitRelationProcessUnitJob::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Job.fullName());
    sel.result(ProcessUnit.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<JobProcessUnitRelationProcessUnitJob::Row>(db, sel);
}
template <> litesql::DataSource<db::Job> JobProcessUnitRelationProcessUnitJob::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Job.fullName());
    sel.where(srcExpr);
    return DataSource<db::Job>(db, db::Job::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::ProcessUnit> JobProcessUnitRelationProcessUnitJob::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(ProcessUnit.fullName());
    sel.where(srcExpr);
    return DataSource<db::ProcessUnit>(db, db::ProcessUnit::Id.in(sel) && expr);
}
UserUserGroupRelationUser2UserGroup::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : userGroup(UserUserGroupRelationUser2UserGroup::UserGroup), user(UserUserGroupRelationUser2UserGroup::User) {
    switch(rec.size()) {
    case 2:
        userGroup = rec[1];
    case 1:
        user = rec[0];
    }
}
const std::string UserUserGroupRelationUser2UserGroup::table__("User_UserGroup_User2UserGroup");
const litesql::FieldType UserUserGroupRelationUser2UserGroup::User("User1_",A_field_type_integer,table__);
const litesql::FieldType UserUserGroupRelationUser2UserGroup::UserGroup("UserGroup2_",A_field_type_integer,table__);
void UserUserGroupRelationUser2UserGroup::link(const litesql::Database& db, const db::User& o0, const db::UserGroup& o1) {
    Record values;
    Split fields;
    fields.push_back(User.name());
    values.push_back(o0.id);
    fields.push_back(UserGroup.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void UserUserGroupRelationUser2UserGroup::unlink(const litesql::Database& db, const db::User& o0, const db::UserGroup& o1) {
    db.delete_(table__, (User == o0.id && UserGroup == o1.id));
}
void UserUserGroupRelationUser2UserGroup::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<UserUserGroupRelationUser2UserGroup::Row> UserUserGroupRelationUser2UserGroup::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(User.fullName());
    sel.result(UserGroup.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<UserUserGroupRelationUser2UserGroup::Row>(db, sel);
}
template <> litesql::DataSource<db::User> UserUserGroupRelationUser2UserGroup::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(User.fullName());
    sel.where(srcExpr);
    return DataSource<db::User>(db, db::User::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::UserGroup> UserUserGroupRelationUser2UserGroup::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(UserGroup.fullName());
    sel.where(srcExpr);
    return DataSource<db::UserGroup>(db, db::UserGroup::Id.in(sel) && expr);
}
JobPartitionRelationJob2Partition::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : partition(JobPartitionRelationJob2Partition::Partition), job(JobPartitionRelationJob2Partition::Job) {
    switch(rec.size()) {
    case 2:
        partition = rec[1];
    case 1:
        job = rec[0];
    }
}
const std::string JobPartitionRelationJob2Partition::table__("Job_Partition_Job2Partition");
const litesql::FieldType JobPartitionRelationJob2Partition::Job("Job1_",A_field_type_integer,table__);
const litesql::FieldType JobPartitionRelationJob2Partition::Partition("Partition2_",A_field_type_integer,table__);
void JobPartitionRelationJob2Partition::link(const litesql::Database& db, const db::Job& o0, const db::Partition& o1) {
    Record values;
    Split fields;
    fields.push_back(Job.name());
    values.push_back(o0.id);
    fields.push_back(Partition.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void JobPartitionRelationJob2Partition::unlink(const litesql::Database& db, const db::Job& o0, const db::Partition& o1) {
    db.delete_(table__, (Job == o0.id && Partition == o1.id));
}
void JobPartitionRelationJob2Partition::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<JobPartitionRelationJob2Partition::Row> JobPartitionRelationJob2Partition::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Job.fullName());
    sel.result(Partition.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<JobPartitionRelationJob2Partition::Row>(db, sel);
}
template <> litesql::DataSource<db::Job> JobPartitionRelationJob2Partition::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Job.fullName());
    sel.where(srcExpr);
    return DataSource<db::Job>(db, db::Job::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::Partition> JobPartitionRelationJob2Partition::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Partition.fullName());
    sel.where(srcExpr);
    return DataSource<db::Partition>(db, db::Partition::Id.in(sel) && expr);
}
JobQueueRelationJob2Queue::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : queue(JobQueueRelationJob2Queue::Queue), job(JobQueueRelationJob2Queue::Job) {
    switch(rec.size()) {
    case 2:
        queue = rec[1];
    case 1:
        job = rec[0];
    }
}
const std::string JobQueueRelationJob2Queue::table__("Job_Queue_Job2Queue");
const litesql::FieldType JobQueueRelationJob2Queue::Job("Job1_",A_field_type_integer,table__);
const litesql::FieldType JobQueueRelationJob2Queue::Queue("Queue2_",A_field_type_integer,table__);
void JobQueueRelationJob2Queue::link(const litesql::Database& db, const db::Job& o0, const db::Queue& o1) {
    Record values;
    Split fields;
    fields.push_back(Job.name());
    values.push_back(o0.id);
    fields.push_back(Queue.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void JobQueueRelationJob2Queue::unlink(const litesql::Database& db, const db::Job& o0, const db::Queue& o1) {
    db.delete_(table__, (Job == o0.id && Queue == o1.id));
}
void JobQueueRelationJob2Queue::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<JobQueueRelationJob2Queue::Row> JobQueueRelationJob2Queue::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Job.fullName());
    sel.result(Queue.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<JobQueueRelationJob2Queue::Row>(db, sel);
}
template <> litesql::DataSource<db::Job> JobQueueRelationJob2Queue::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Job.fullName());
    sel.where(srcExpr);
    return DataSource<db::Job>(db, db::Job::Id.in(sel) && expr);
}
template <> litesql::DataSource<db::Queue> JobQueueRelationJob2Queue::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Queue.fullName());
    sel.where(srcExpr);
    return DataSource<db::Queue>(db, db::Queue::Id.in(sel) && expr);
}
const litesql::FieldType Project::Own::Id("id_",A_field_type_integer,"Project_");
Project::FilterHandle::FilterHandle(const Project& owner)
         : litesql::RelationHandle<Project>(owner) {
}
void Project::FilterHandle::link(const Filter& o0) {
    FilterProjectRelation::link(owner->getDatabase(), o0, *owner);
}
void Project::FilterHandle::unlink(const Filter& o0) {
    FilterProjectRelation::unlink(owner->getDatabase(), o0, *owner);
}
void Project::FilterHandle::del(const litesql::Expr& expr) {
    FilterProjectRelation::del(owner->getDatabase(), expr && FilterProjectRelation::Project == owner->id);
}
litesql::DataSource<Filter> Project::FilterHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FilterProjectRelation::get<Filter>(owner->getDatabase(), expr, (FilterProjectRelation::Project == owner->id) && srcExpr);
}
litesql::DataSource<FilterProjectRelation::Row> Project::FilterHandle::getRows(const litesql::Expr& expr) {
    return FilterProjectRelation::getRows(owner->getDatabase(), expr && (FilterProjectRelation::Project == owner->id));
}
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
Project::PresetsHandle::PresetsHandle(const Project& owner)
         : litesql::RelationHandle<Project>(owner) {
}
void Project::PresetsHandle::link(const Preset& o0) {
    PresetProjectRelation::link(owner->getDatabase(), o0, *owner);
}
void Project::PresetsHandle::unlink(const Preset& o0) {
    PresetProjectRelation::unlink(owner->getDatabase(), o0, *owner);
}
void Project::PresetsHandle::del(const litesql::Expr& expr) {
    PresetProjectRelation::del(owner->getDatabase(), expr && PresetProjectRelation::Project == owner->id);
}
litesql::DataSource<Preset> Project::PresetsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return PresetProjectRelation::get<Preset>(owner->getDatabase(), expr, (PresetProjectRelation::Project == owner->id) && srcExpr);
}
litesql::DataSource<PresetProjectRelation::Row> Project::PresetsHandle::getRows(const litesql::Expr& expr) {
    return PresetProjectRelation::getRows(owner->getDatabase(), expr && (PresetProjectRelation::Project == owner->id));
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
const litesql::FieldType Project::Id("id_",A_field_type_integer,table__);
const litesql::FieldType Project::Type("type_",A_field_type_string,table__);
const litesql::FieldType Project::Name("name_",A_field_type_string,table__);
const litesql::FieldType Project::Outdirectory("outdirectory_",A_field_type_string,table__);
const litesql::FieldType Project::Status("status_",A_field_type_string,table__);
const litesql::FieldType Project::Created("created_",A_field_type_date,table__);
const litesql::FieldType Project::Started("started_",A_field_type_date,table__);
const litesql::FieldType Project::Completed("completed_",A_field_type_date,table__);
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
Project::FilterHandle Project::filter() {
    return Project::FilterHandle(*this);
}
Project::MediafilesHandle Project::mediafiles() {
    return Project::MediafilesHandle(*this);
}
Project::PresetsHandle Project::presets() {
    return Project::PresetsHandle(*this);
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
    FilterProjectRelation::del(*db, (FilterProjectRelation::Project == id));
    MediaFileProjectRelation::del(*db, (MediaFileProjectRelation::Project == id));
    PresetProjectRelation::del(*db, (PresetProjectRelation::Project == id));
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
    if (!typeIsCorrect()) {
        std::auto_ptr<Project> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Project::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<Project> Project::upcast() const {
    return auto_ptr<Project>(new Project(*this));
}
std::auto_ptr<Project> Project::upcastCopy() const {
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
const litesql::FieldType Filter::Own::Id("id_",A_field_type_integer,"Filter_");
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
Filter::MediafileHandle::MediafileHandle(const Filter& owner)
         : litesql::RelationHandle<Filter>(owner) {
}
void Filter::MediafileHandle::link(const MediaFile& o0) {
    FilterMediaFileRelation::link(owner->getDatabase(), *owner, o0);
}
void Filter::MediafileHandle::unlink(const MediaFile& o0) {
    FilterMediaFileRelation::unlink(owner->getDatabase(), *owner, o0);
}
void Filter::MediafileHandle::del(const litesql::Expr& expr) {
    FilterMediaFileRelation::del(owner->getDatabase(), expr && FilterMediaFileRelation::Filter == owner->id);
}
litesql::DataSource<MediaFile> Filter::MediafileHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FilterMediaFileRelation::get<MediaFile>(owner->getDatabase(), expr, (FilterMediaFileRelation::Filter == owner->id) && srcExpr);
}
litesql::DataSource<FilterMediaFileRelation::Row> Filter::MediafileHandle::getRows(const litesql::Expr& expr) {
    return FilterMediaFileRelation::getRows(owner->getDatabase(), expr && (FilterMediaFileRelation::Filter == owner->id));
}
Filter::ProjectHandle::ProjectHandle(const Filter& owner)
         : litesql::RelationHandle<Filter>(owner) {
}
void Filter::ProjectHandle::link(const Project& o0) {
    FilterProjectRelation::link(owner->getDatabase(), *owner, o0);
}
void Filter::ProjectHandle::unlink(const Project& o0) {
    FilterProjectRelation::unlink(owner->getDatabase(), *owner, o0);
}
void Filter::ProjectHandle::del(const litesql::Expr& expr) {
    FilterProjectRelation::del(owner->getDatabase(), expr && FilterProjectRelation::Filter == owner->id);
}
litesql::DataSource<Project> Filter::ProjectHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FilterProjectRelation::get<Project>(owner->getDatabase(), expr, (FilterProjectRelation::Filter == owner->id) && srcExpr);
}
litesql::DataSource<FilterProjectRelation::Row> Filter::ProjectHandle::getRows(const litesql::Expr& expr) {
    return FilterProjectRelation::getRows(owner->getDatabase(), expr && (FilterProjectRelation::Filter == owner->id));
}
Filter::ProfileHandle::ProfileHandle(const Filter& owner)
         : litesql::RelationHandle<Filter>(owner) {
}
void Filter::ProfileHandle::link(const Profile& o0) {
    FilterProfileRelation::link(owner->getDatabase(), *owner, o0);
}
void Filter::ProfileHandle::unlink(const Profile& o0) {
    FilterProfileRelation::unlink(owner->getDatabase(), *owner, o0);
}
void Filter::ProfileHandle::del(const litesql::Expr& expr) {
    FilterProfileRelation::del(owner->getDatabase(), expr && FilterProfileRelation::Filter == owner->id);
}
litesql::DataSource<Profile> Filter::ProfileHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FilterProfileRelation::get<Profile>(owner->getDatabase(), expr, (FilterProfileRelation::Filter == owner->id) && srcExpr);
}
litesql::DataSource<FilterProfileRelation::Row> Filter::ProfileHandle::getRows(const litesql::Expr& expr) {
    return FilterProfileRelation::getRows(owner->getDatabase(), expr && (FilterProfileRelation::Filter == owner->id));
}
Filter::StreamHandle::StreamHandle(const Filter& owner)
         : litesql::RelationHandle<Filter>(owner) {
}
void Filter::StreamHandle::link(const Stream& o0) {
    FilterStreamRelation::link(owner->getDatabase(), *owner, o0);
}
void Filter::StreamHandle::unlink(const Stream& o0) {
    FilterStreamRelation::unlink(owner->getDatabase(), *owner, o0);
}
void Filter::StreamHandle::del(const litesql::Expr& expr) {
    FilterStreamRelation::del(owner->getDatabase(), expr && FilterStreamRelation::Filter == owner->id);
}
litesql::DataSource<Stream> Filter::StreamHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FilterStreamRelation::get<Stream>(owner->getDatabase(), expr, (FilterStreamRelation::Filter == owner->id) && srcExpr);
}
litesql::DataSource<FilterStreamRelation::Row> Filter::StreamHandle::getRows(const litesql::Expr& expr) {
    return FilterStreamRelation::getRows(owner->getDatabase(), expr && (FilterStreamRelation::Filter == owner->id));
}
const std::string Filter::type__("Filter");
const std::string Filter::table__("Filter_");
const std::string Filter::sequence__("Filter_seq");
const litesql::FieldType Filter::Id("id_",A_field_type_integer,table__);
const litesql::FieldType Filter::Type("type_",A_field_type_string,table__);
const litesql::FieldType Filter::Filtername("filtername_",A_field_type_string,table__);
const litesql::FieldType Filter::Filterid("filterid_",A_field_type_string,table__);
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
Filter::MediafileHandle Filter::mediafile() {
    return Filter::MediafileHandle(*this);
}
Filter::ProjectHandle Filter::project() {
    return Filter::ProjectHandle(*this);
}
Filter::ProfileHandle Filter::profile() {
    return Filter::ProfileHandle(*this);
}
Filter::StreamHandle Filter::stream() {
    return Filter::StreamHandle(*this);
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
    FilterMediaFileRelation::del(*db, (FilterMediaFileRelation::Filter == id));
    FilterProjectRelation::del(*db, (FilterProjectRelation::Filter == id));
    FilterProfileRelation::del(*db, (FilterProfileRelation::Filter == id));
    FilterStreamRelation::del(*db, (FilterStreamRelation::Filter == id));
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
    if (!typeIsCorrect()) {
        std::auto_ptr<Filter> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Filter::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<Filter> Filter::upcast() const {
    return auto_ptr<Filter>(new Filter(*this));
}
std::auto_ptr<Filter> Filter::upcastCopy() const {
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
const litesql::FieldType FilterParameter::Own::Id("id_",A_field_type_integer,"FilterParameter_");
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
const litesql::FieldType FilterParameter::Id("id_",A_field_type_integer,table__);
const litesql::FieldType FilterParameter::Type("type_",A_field_type_string,table__);
const litesql::FieldType FilterParameter::Fkey("fkey_",A_field_type_string,table__);
const litesql::FieldType FilterParameter::Fval("fval_",A_field_type_string,table__);
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
    if (!typeIsCorrect()) {
        std::auto_ptr<FilterParameter> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool FilterParameter::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<FilterParameter> FilterParameter::upcast() const {
    return auto_ptr<FilterParameter>(new FilterParameter(*this));
}
std::auto_ptr<FilterParameter> FilterParameter::upcastCopy() const {
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
const litesql::FieldType MediaFile::Own::Id("id_",A_field_type_integer,"MediaFile_");
MediaFile::FilterHandle::FilterHandle(const MediaFile& owner)
         : litesql::RelationHandle<MediaFile>(owner) {
}
void MediaFile::FilterHandle::link(const Filter& o0) {
    FilterMediaFileRelation::link(owner->getDatabase(), o0, *owner);
}
void MediaFile::FilterHandle::unlink(const Filter& o0) {
    FilterMediaFileRelation::unlink(owner->getDatabase(), o0, *owner);
}
void MediaFile::FilterHandle::del(const litesql::Expr& expr) {
    FilterMediaFileRelation::del(owner->getDatabase(), expr && FilterMediaFileRelation::MediaFile == owner->id);
}
litesql::DataSource<Filter> MediaFile::FilterHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FilterMediaFileRelation::get<Filter>(owner->getDatabase(), expr, (FilterMediaFileRelation::MediaFile == owner->id) && srcExpr);
}
litesql::DataSource<FilterMediaFileRelation::Row> MediaFile::FilterHandle::getRows(const litesql::Expr& expr) {
    return FilterMediaFileRelation::getRows(owner->getDatabase(), expr && (FilterMediaFileRelation::MediaFile == owner->id));
}
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
const litesql::FieldType MediaFile::Id("id_",A_field_type_integer,table__);
const litesql::FieldType MediaFile::Type("type_",A_field_type_string,table__);
const litesql::FieldType MediaFile::Filename("filename_",A_field_type_string,table__);
const litesql::FieldType MediaFile::Path("path_",A_field_type_string,table__);
const litesql::FieldType MediaFile::Filesize("filesize_",A_field_type_double,table__);
const litesql::FieldType MediaFile::Streamcount("streamcount_",A_field_type_integer,table__);
const litesql::FieldType MediaFile::Containertype("containertype_",A_field_type_string,table__);
const litesql::FieldType MediaFile::Duration("duration_",A_field_type_double,table__);
const litesql::FieldType MediaFile::Starttime("starttime_",A_field_type_double,table__);
const litesql::FieldType MediaFile::Bitrate("bitrate_",A_field_type_integer,table__);
const litesql::FieldType MediaFile::Created("created_",A_field_type_datetime,table__);
const litesql::FieldType MediaFile::Filetype("filetype_",A_field_type_integer,table__);
const litesql::FieldType MediaFile::Parent("parent_",A_field_type_integer,table__);
const litesql::FieldType MediaFile::Metatitle("metatitle_",A_field_type_string,table__);
const litesql::FieldType MediaFile::Metaauthor("metaauthor_",A_field_type_string,table__);
const litesql::FieldType MediaFile::Metacopyright("metacopyright_",A_field_type_string,table__);
const litesql::FieldType MediaFile::Metacomment("metacomment_",A_field_type_string,table__);
const litesql::FieldType MediaFile::Metaalbum("metaalbum_",A_field_type_string,table__);
const litesql::FieldType MediaFile::Metayear("metayear_",A_field_type_integer,table__);
const litesql::FieldType MediaFile::Metatrack("metatrack_",A_field_type_integer,table__);
const litesql::FieldType MediaFile::Metagenre("metagenre_",A_field_type_integer,table__);
void MediaFile::defaults() {
    id = 0;
    filesize = 0.0;
    streamcount = 0;
    duration = 0.0;
    starttime = 0.0;
    bitrate = 0;
    created = 0;
    filetype = 0;
    parent = 0;
    metayear = 0;
    metatrack = 0;
    metagenre = 0;
}
MediaFile::MediaFile(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), filename(Filename), path(Path), filesize(Filesize), streamcount(Streamcount), containertype(Containertype), duration(Duration), starttime(Starttime), bitrate(Bitrate), created(Created), filetype(Filetype), parent(Parent), metatitle(Metatitle), metaauthor(Metaauthor), metacopyright(Metacopyright), metacomment(Metacomment), metaalbum(Metaalbum), metayear(Metayear), metatrack(Metatrack), metagenre(Metagenre) {
    defaults();
}
MediaFile::MediaFile(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), filename(Filename), path(Path), filesize(Filesize), streamcount(Streamcount), containertype(Containertype), duration(Duration), starttime(Starttime), bitrate(Bitrate), created(Created), filetype(Filetype), parent(Parent), metatitle(Metatitle), metaauthor(Metaauthor), metacopyright(Metacopyright), metacomment(Metacomment), metaalbum(Metaalbum), metayear(Metayear), metatrack(Metatrack), metagenre(Metagenre) {
    defaults();
    size_t size = (rec.size() > 21) ? 21 : rec.size();
    switch(size) {
    case 21: metagenre = convert<const std::string&, int>(rec[20]);
        metagenre.setModified(false);
    case 20: metatrack = convert<const std::string&, int>(rec[19]);
        metatrack.setModified(false);
    case 19: metayear = convert<const std::string&, int>(rec[18]);
        metayear.setModified(false);
    case 18: metaalbum = convert<const std::string&, std::string>(rec[17]);
        metaalbum.setModified(false);
    case 17: metacomment = convert<const std::string&, std::string>(rec[16]);
        metacomment.setModified(false);
    case 16: metacopyright = convert<const std::string&, std::string>(rec[15]);
        metacopyright.setModified(false);
    case 15: metaauthor = convert<const std::string&, std::string>(rec[14]);
        metaauthor.setModified(false);
    case 14: metatitle = convert<const std::string&, std::string>(rec[13]);
        metatitle.setModified(false);
    case 13: parent = convert<const std::string&, int>(rec[12]);
        parent.setModified(false);
    case 12: filetype = convert<const std::string&, int>(rec[11]);
        filetype.setModified(false);
    case 11: created = convert<const std::string&, litesql::DateTime>(rec[10]);
        created.setModified(false);
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
     : litesql::Persistent(obj), id(obj.id), type(obj.type), filename(obj.filename), path(obj.path), filesize(obj.filesize), streamcount(obj.streamcount), containertype(obj.containertype), duration(obj.duration), starttime(obj.starttime), bitrate(obj.bitrate), created(obj.created), filetype(obj.filetype), parent(obj.parent), metatitle(obj.metatitle), metaauthor(obj.metaauthor), metacopyright(obj.metacopyright), metacomment(obj.metacomment), metaalbum(obj.metaalbum), metayear(obj.metayear), metatrack(obj.metatrack), metagenre(obj.metagenre) {
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
        starttime = obj.starttime;
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
MediaFile::FilterHandle MediaFile::filter() {
    return MediaFile::FilterHandle(*this);
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
    fields.push_back(starttime.name());
    values.push_back(starttime);
    starttime.setModified(false);
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
    updateField(updates, table__, starttime);
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
    ftypes.push_back(Starttime);
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
    FilterMediaFileRelation::del(*db, (FilterMediaFileRelation::MediaFile == id));
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
    if (!typeIsCorrect()) {
        std::auto_ptr<MediaFile> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool MediaFile::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<MediaFile> MediaFile::upcast() const {
    return auto_ptr<MediaFile>(new MediaFile(*this));
}
std::auto_ptr<MediaFile> MediaFile::upcastCopy() const {
    MediaFile* np = new MediaFile(*this);
    np->id = id;
    np->type = type;
    np->filename = filename;
    np->path = path;
    np->filesize = filesize;
    np->streamcount = streamcount;
    np->containertype = containertype;
    np->duration = duration;
    np->starttime = starttime;
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
    os << o.starttime.name() << " = " << o.starttime << std::endl;
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
const litesql::FieldType OutputFile::Own::Id("id_",A_field_type_integer,"OutputFile_");
const std::string OutputFile::StatusType::Waiting("WAITING");
const std::string OutputFile::StatusType::Processing("PROCESSING");
const std::string OutputFile::StatusType::Error("ERROR");
const std::string OutputFile::StatusType::Completed("COMPLETED");
OutputFile::StatusType::StatusType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals)
         : litesql::FieldType(n,t,tbl,vals) {
}
const std::string OutputFile::Status::Waiting("WAITING");
const std::string OutputFile::Status::Processing("PROCESSING");
const std::string OutputFile::Status::Error("ERROR");
const std::string OutputFile::Status::Completed("COMPLETED");
const std::string OutputFile::type__("OutputFile");
const std::string OutputFile::table__("OutputFile_");
const std::string OutputFile::sequence__("OutputFile_seq");
const litesql::FieldType OutputFile::Id("id_",A_field_type_integer,table__);
const litesql::FieldType OutputFile::Type("type_",A_field_type_string,table__);
const litesql::FieldType OutputFile::Filename("filename_",A_field_type_string,table__);
const litesql::FieldType OutputFile::Path("path_",A_field_type_string,table__);
const litesql::FieldType OutputFile::Jobid("jobid_",A_field_type_string,table__);
const litesql::FieldType OutputFile::Outfiledata("outfiledata_",A_field_type_string,table__);
std::vector < std::pair< std::string, std::string > > OutputFile::status_values;
const OutputFile::StatusType OutputFile::Status("status_",A_field_type_string,table__,status_values);
const OutputFile::StatusType OutputFile::Uuid("uuid_",A_field_type_string,table__);
void OutputFile::initValues() {
    status_values.clear();
    status_values.push_back(make_pair<std::string, std::string>("Waiting","WAITING"));
    status_values.push_back(make_pair<std::string, std::string>("Processing","PROCESSING"));
    status_values.push_back(make_pair<std::string, std::string>("Error","ERROR"));
    status_values.push_back(make_pair<std::string, std::string>("Completed","COMPLETED"));
}
void OutputFile::defaults() {
    id = 0;
}
OutputFile::OutputFile(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), filename(Filename), path(Path), jobid(Jobid), outfiledata(Outfiledata), status(Status), uuid(Uuid) {
    defaults();
}
OutputFile::OutputFile(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), filename(Filename), path(Path), jobid(Jobid), outfiledata(Outfiledata), status(Status), uuid(Uuid) {
    defaults();
    size_t size = (rec.size() > 8) ? 8 : rec.size();
    switch(size) {
    case 8: uuid = convert<const std::string&, std::string>(rec[7]);
        uuid.setModified(false);
    case 7: status = convert<const std::string&, std::string>(rec[6]);
        status.setModified(false);
    case 6: outfiledata = convert<const std::string&, std::string>(rec[5]);
        outfiledata.setModified(false);
    case 5: jobid = convert<const std::string&, std::string>(rec[4]);
        jobid.setModified(false);
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
OutputFile::OutputFile(const OutputFile& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), filename(obj.filename), path(obj.path), jobid(obj.jobid), outfiledata(obj.outfiledata), status(obj.status), uuid(obj.uuid) {
}
const OutputFile& OutputFile::operator=(const OutputFile& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        filename = obj.filename;
        path = obj.path;
        jobid = obj.jobid;
        outfiledata = obj.outfiledata;
        status = obj.status;
        uuid = obj.uuid;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string OutputFile::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
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
    fields.push_back(jobid.name());
    values.push_back(jobid);
    jobid.setModified(false);
    fields.push_back(outfiledata.name());
    values.push_back(outfiledata);
    outfiledata.setModified(false);
    fields.push_back(status.name());
    values.push_back(status);
    status.setModified(false);
    fields.push_back(uuid.name());
    values.push_back(uuid);
    uuid.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void OutputFile::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void OutputFile::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, filename);
    updateField(updates, table__, path);
    updateField(updates, table__, jobid);
    updateField(updates, table__, outfiledata);
    updateField(updates, table__, status);
    updateField(updates, table__, uuid);
}
void OutputFile::addIDUpdates(Updates& updates) {
}
void OutputFile::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Filename);
    ftypes.push_back(Path);
    ftypes.push_back(Jobid);
    ftypes.push_back(Outfiledata);
    ftypes.push_back(Status);
    ftypes.push_back(Uuid);
}
void OutputFile::delRecord() {
    deleteFromTable(table__, id);
}
void OutputFile::delRelations() {
}
void OutputFile::update() {
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
void OutputFile::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<OutputFile> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool OutputFile::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<OutputFile> OutputFile::upcast() const {
    return auto_ptr<OutputFile>(new OutputFile(*this));
}
std::auto_ptr<OutputFile> OutputFile::upcastCopy() const {
    OutputFile* np = new OutputFile(*this);
    np->id = id;
    np->type = type;
    np->filename = filename;
    np->path = path;
    np->jobid = jobid;
    np->outfiledata = outfiledata;
    np->status = status;
    np->uuid = uuid;
    np->inDatabase = inDatabase;
    return auto_ptr<OutputFile>(np);
}
std::ostream & operator<<(std::ostream& os, OutputFile o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.filename.name() << " = " << o.filename << std::endl;
    os << o.path.name() << " = " << o.path << std::endl;
    os << o.jobid.name() << " = " << o.jobid << std::endl;
    os << o.outfiledata.name() << " = " << o.outfiledata << std::endl;
    os << o.status.name() << " = " << o.status << std::endl;
    os << o.uuid.name() << " = " << o.uuid << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType ProfileGroup::Own::Id("id_",A_field_type_integer,"ProfileGroup_");
ProfileGroup::ProfilesHandle::ProfilesHandle(const ProfileGroup& owner)
         : litesql::RelationHandle<ProfileGroup>(owner) {
}
void ProfileGroup::ProfilesHandle::link(const Profile& o0) {
    ProfileGroup2Profile::link(owner->getDatabase(), o0, *owner);
}
void ProfileGroup::ProfilesHandle::unlink(const Profile& o0) {
    ProfileGroup2Profile::unlink(owner->getDatabase(), o0, *owner);
}
void ProfileGroup::ProfilesHandle::del(const litesql::Expr& expr) {
    ProfileGroup2Profile::del(owner->getDatabase(), expr && ProfileGroup2Profile::ProfileGroup == owner->id);
}
litesql::DataSource<Profile> ProfileGroup::ProfilesHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return ProfileGroup2Profile::get<Profile>(owner->getDatabase(), expr, (ProfileGroup2Profile::ProfileGroup == owner->id) && srcExpr);
}
litesql::DataSource<ProfileGroup2Profile::Row> ProfileGroup::ProfilesHandle::getRows(const litesql::Expr& expr) {
    return ProfileGroup2Profile::getRows(owner->getDatabase(), expr && (ProfileGroup2Profile::ProfileGroup == owner->id));
}
ProfileGroup::ChildrensHandle::ChildrensHandle(const ProfileGroup& owner)
         : litesql::RelationHandle<ProfileGroup>(owner) {
}
void ProfileGroup::ChildrensHandle::link(const ProfileGroup& o0) {
    ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::link(owner->getDatabase(), *owner, o0);
}
void ProfileGroup::ChildrensHandle::unlink(const ProfileGroup& o0) {
    ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::unlink(owner->getDatabase(), *owner, o0);
}
void ProfileGroup::ChildrensHandle::del(const litesql::Expr& expr) {
    ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::del(owner->getDatabase(), expr && ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::ProfileGroup1 == owner->id);
}
litesql::DataSource<ProfileGroup> ProfileGroup::ChildrensHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::getProfileGroup2(owner->getDatabase(), expr, (ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::ProfileGroup1 == owner->id) && srcExpr);
}
litesql::DataSource<ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::Row> ProfileGroup::ChildrensHandle::getRows(const litesql::Expr& expr) {
    return ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::getRows(owner->getDatabase(), expr && (ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::ProfileGroup1 == owner->id));
}
ProfileGroup::ParentHandle::ParentHandle(const ProfileGroup& owner)
         : litesql::RelationHandle<ProfileGroup>(owner) {
}
void ProfileGroup::ParentHandle::link(const ProfileGroup& o0) {
    ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::link(owner->getDatabase(), o0, *owner);
}
void ProfileGroup::ParentHandle::unlink(const ProfileGroup& o0) {
    ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::unlink(owner->getDatabase(), o0, *owner);
}
void ProfileGroup::ParentHandle::del(const litesql::Expr& expr) {
    ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::del(owner->getDatabase(), expr && ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::ProfileGroup2 == owner->id);
}
litesql::DataSource<ProfileGroup> ProfileGroup::ParentHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::getProfileGroup1(owner->getDatabase(), expr, (ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::ProfileGroup2 == owner->id) && srcExpr);
}
litesql::DataSource<ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::Row> ProfileGroup::ParentHandle::getRows(const litesql::Expr& expr) {
    return ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::getRows(owner->getDatabase(), expr && (ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::ProfileGroup2 == owner->id));
}
const std::string ProfileGroup::type__("ProfileGroup");
const std::string ProfileGroup::table__("ProfileGroup_");
const std::string ProfileGroup::sequence__("ProfileGroup_seq");
const litesql::FieldType ProfileGroup::Id("id_",A_field_type_integer,table__);
const litesql::FieldType ProfileGroup::Type("type_",A_field_type_string,table__);
const litesql::FieldType ProfileGroup::Name("name_",A_field_type_string,table__);
void ProfileGroup::defaults() {
    id = 0;
}
ProfileGroup::ProfileGroup(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name) {
    defaults();
}
ProfileGroup::ProfileGroup(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name) {
    defaults();
    size_t size = (rec.size() > 3) ? 3 : rec.size();
    switch(size) {
    case 3: name = convert<const std::string&, std::string>(rec[2]);
        name.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
ProfileGroup::ProfileGroup(const ProfileGroup& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name) {
}
const ProfileGroup& ProfileGroup::operator=(const ProfileGroup& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
ProfileGroup::ProfilesHandle ProfileGroup::profiles() {
    return ProfileGroup::ProfilesHandle(*this);
}
ProfileGroup::ChildrensHandle ProfileGroup::childrens() {
    return ProfileGroup::ChildrensHandle(*this);
}
ProfileGroup::ParentHandle ProfileGroup::parent() {
    return ProfileGroup::ParentHandle(*this);
}
std::string ProfileGroup::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
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
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void ProfileGroup::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void ProfileGroup::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, name);
}
void ProfileGroup::addIDUpdates(Updates& updates) {
}
void ProfileGroup::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
}
void ProfileGroup::delRecord() {
    deleteFromTable(table__, id);
}
void ProfileGroup::delRelations() {
    ProfileGroup2Profile::del(*db, (ProfileGroup2Profile::ProfileGroup == id));
    ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::del(*db, (ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::ProfileGroup1 == id) || (ProfileGroupProfileGroupRelationProfileGroup2ProfileGroupChildren::ProfileGroup2 == id));
}
void ProfileGroup::update() {
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
void ProfileGroup::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<ProfileGroup> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool ProfileGroup::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<ProfileGroup> ProfileGroup::upcast() const {
    return auto_ptr<ProfileGroup>(new ProfileGroup(*this));
}
std::auto_ptr<ProfileGroup> ProfileGroup::upcastCopy() const {
    ProfileGroup* np = new ProfileGroup(*this);
    np->id = id;
    np->type = type;
    np->name = name;
    np->inDatabase = inDatabase;
    return auto_ptr<ProfileGroup>(np);
}
std::ostream & operator<<(std::ostream& os, ProfileGroup o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Profile::Own::Id("id_",A_field_type_integer,"Profile_");
Profile::FilterHandle::FilterHandle(const Profile& owner)
         : litesql::RelationHandle<Profile>(owner) {
}
void Profile::FilterHandle::link(const Filter& o0) {
    FilterProfileRelation::link(owner->getDatabase(), o0, *owner);
}
void Profile::FilterHandle::unlink(const Filter& o0) {
    FilterProfileRelation::unlink(owner->getDatabase(), o0, *owner);
}
void Profile::FilterHandle::del(const litesql::Expr& expr) {
    FilterProfileRelation::del(owner->getDatabase(), expr && FilterProfileRelation::Profile == owner->id);
}
litesql::DataSource<Filter> Profile::FilterHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FilterProfileRelation::get<Filter>(owner->getDatabase(), expr, (FilterProfileRelation::Profile == owner->id) && srcExpr);
}
litesql::DataSource<FilterProfileRelation::Row> Profile::FilterHandle::getRows(const litesql::Expr& expr) {
    return FilterProfileRelation::getRows(owner->getDatabase(), expr && (FilterProfileRelation::Profile == owner->id));
}
Profile::GroupHandle::GroupHandle(const Profile& owner)
         : litesql::RelationHandle<Profile>(owner) {
}
void Profile::GroupHandle::link(const ProfileGroup& o0) {
    ProfileGroup2Profile::link(owner->getDatabase(), *owner, o0);
}
void Profile::GroupHandle::unlink(const ProfileGroup& o0) {
    ProfileGroup2Profile::unlink(owner->getDatabase(), *owner, o0);
}
void Profile::GroupHandle::del(const litesql::Expr& expr) {
    ProfileGroup2Profile::del(owner->getDatabase(), expr && ProfileGroup2Profile::Profile == owner->id);
}
litesql::DataSource<ProfileGroup> Profile::GroupHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return ProfileGroup2Profile::get<ProfileGroup>(owner->getDatabase(), expr, (ProfileGroup2Profile::Profile == owner->id) && srcExpr);
}
litesql::DataSource<ProfileGroup2Profile::Row> Profile::GroupHandle::getRows(const litesql::Expr& expr) {
    return ProfileGroup2Profile::getRows(owner->getDatabase(), expr && (ProfileGroup2Profile::Profile == owner->id));
}
Profile::ParamsHandle::ParamsHandle(const Profile& owner)
         : litesql::RelationHandle<Profile>(owner) {
}
void Profile::ParamsHandle::link(const ProfileParameter& o0) {
    ProfileProfileParameterRelation::link(owner->getDatabase(), *owner, o0);
}
void Profile::ParamsHandle::unlink(const ProfileParameter& o0) {
    ProfileProfileParameterRelation::unlink(owner->getDatabase(), *owner, o0);
}
void Profile::ParamsHandle::del(const litesql::Expr& expr) {
    ProfileProfileParameterRelation::del(owner->getDatabase(), expr && ProfileProfileParameterRelation::Profile == owner->id);
}
litesql::DataSource<ProfileParameter> Profile::ParamsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return ProfileProfileParameterRelation::get<ProfileParameter>(owner->getDatabase(), expr, (ProfileProfileParameterRelation::Profile == owner->id) && srcExpr);
}
litesql::DataSource<ProfileProfileParameterRelation::Row> Profile::ParamsHandle::getRows(const litesql::Expr& expr) {
    return ProfileProfileParameterRelation::getRows(owner->getDatabase(), expr && (ProfileProfileParameterRelation::Profile == owner->id));
}
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
Profile::VpresetHandle::VpresetHandle(const Profile& owner)
         : litesql::RelationHandle<Profile>(owner) {
}
void Profile::VpresetHandle::link(const CodecPreset& o0) {
    CodecPresetProfileRelationVideoCodecPreset2Profile::link(owner->getDatabase(), o0, *owner);
}
void Profile::VpresetHandle::unlink(const CodecPreset& o0) {
    CodecPresetProfileRelationVideoCodecPreset2Profile::unlink(owner->getDatabase(), o0, *owner);
}
void Profile::VpresetHandle::del(const litesql::Expr& expr) {
    CodecPresetProfileRelationVideoCodecPreset2Profile::del(owner->getDatabase(), expr && CodecPresetProfileRelationVideoCodecPreset2Profile::Profile == owner->id);
}
litesql::DataSource<CodecPreset> Profile::VpresetHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return CodecPresetProfileRelationVideoCodecPreset2Profile::get<CodecPreset>(owner->getDatabase(), expr, (CodecPresetProfileRelationVideoCodecPreset2Profile::Profile == owner->id) && srcExpr);
}
litesql::DataSource<CodecPresetProfileRelationVideoCodecPreset2Profile::Row> Profile::VpresetHandle::getRows(const litesql::Expr& expr) {
    return CodecPresetProfileRelationVideoCodecPreset2Profile::getRows(owner->getDatabase(), expr && (CodecPresetProfileRelationVideoCodecPreset2Profile::Profile == owner->id));
}
Profile::ApresetHandle::ApresetHandle(const Profile& owner)
         : litesql::RelationHandle<Profile>(owner) {
}
void Profile::ApresetHandle::link(const CodecPreset& o0) {
    CodecPresetProfileRelationAudioCodecPreset2Profile::link(owner->getDatabase(), o0, *owner);
}
void Profile::ApresetHandle::unlink(const CodecPreset& o0) {
    CodecPresetProfileRelationAudioCodecPreset2Profile::unlink(owner->getDatabase(), o0, *owner);
}
void Profile::ApresetHandle::del(const litesql::Expr& expr) {
    CodecPresetProfileRelationAudioCodecPreset2Profile::del(owner->getDatabase(), expr && CodecPresetProfileRelationAudioCodecPreset2Profile::Profile == owner->id);
}
litesql::DataSource<CodecPreset> Profile::ApresetHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return CodecPresetProfileRelationAudioCodecPreset2Profile::get<CodecPreset>(owner->getDatabase(), expr, (CodecPresetProfileRelationAudioCodecPreset2Profile::Profile == owner->id) && srcExpr);
}
litesql::DataSource<CodecPresetProfileRelationAudioCodecPreset2Profile::Row> Profile::ApresetHandle::getRows(const litesql::Expr& expr) {
    return CodecPresetProfileRelationAudioCodecPreset2Profile::getRows(owner->getDatabase(), expr && (CodecPresetProfileRelationAudioCodecPreset2Profile::Profile == owner->id));
}
const std::string Profile::type__("Profile");
const std::string Profile::table__("Profile_");
const std::string Profile::sequence__("Profile_seq");
const litesql::FieldType Profile::Id("id_",A_field_type_integer,table__);
const litesql::FieldType Profile::Type("type_",A_field_type_string,table__);
const litesql::FieldType Profile::Name("name_",A_field_type_string,table__);
const litesql::FieldType Profile::Created("created_",A_field_type_datetime,table__);
const litesql::FieldType Profile::Format("format_",A_field_type_string,table__);
const litesql::FieldType Profile::Formatext("formatext_",A_field_type_string,table__);
const litesql::FieldType Profile::Vcodec("vcodec_",A_field_type_integer,table__);
const litesql::FieldType Profile::Vbitrate("vbitrate_",A_field_type_integer,table__);
const litesql::FieldType Profile::Vframerate("vframerate_",A_field_type_string,table__);
const litesql::FieldType Profile::Vwidth("vwidth_",A_field_type_integer,table__);
const litesql::FieldType Profile::Vheight("vheight_",A_field_type_integer,table__);
const litesql::FieldType Profile::Vextra("vextra_",A_field_type_string,table__);
const litesql::FieldType Profile::Achannels("achannels_",A_field_type_integer,table__);
const litesql::FieldType Profile::Acodec("acodec_",A_field_type_integer,table__);
const litesql::FieldType Profile::Abitrate("abitrate_",A_field_type_integer,table__);
const litesql::FieldType Profile::Asamplerate("asamplerate_",A_field_type_integer,table__);
const litesql::FieldType Profile::Aextra("aextra_",A_field_type_string,table__);
const litesql::FieldType Profile::Profiletype("profiletype_",A_field_type_integer,table__);
const litesql::FieldType Profile::Deinterlace("deinterlace_",A_field_type_integer,table__);
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
    deinterlace = 0;
}
Profile::Profile(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name), created(Created), format(Format), formatext(Formatext), vcodec(Vcodec), vbitrate(Vbitrate), vframerate(Vframerate), vwidth(Vwidth), vheight(Vheight), vextra(Vextra), achannels(Achannels), acodec(Acodec), abitrate(Abitrate), asamplerate(Asamplerate), aextra(Aextra), profiletype(Profiletype), deinterlace(Deinterlace) {
    defaults();
}
Profile::Profile(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name), created(Created), format(Format), formatext(Formatext), vcodec(Vcodec), vbitrate(Vbitrate), vframerate(Vframerate), vwidth(Vwidth), vheight(Vheight), vextra(Vextra), achannels(Achannels), acodec(Acodec), abitrate(Abitrate), asamplerate(Asamplerate), aextra(Aextra), profiletype(Profiletype), deinterlace(Deinterlace) {
    defaults();
    size_t size = (rec.size() > 19) ? 19 : rec.size();
    switch(size) {
    case 19: deinterlace = convert<const std::string&, int>(rec[18]);
        deinterlace.setModified(false);
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
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name), created(obj.created), format(obj.format), formatext(obj.formatext), vcodec(obj.vcodec), vbitrate(obj.vbitrate), vframerate(obj.vframerate), vwidth(obj.vwidth), vheight(obj.vheight), vextra(obj.vextra), achannels(obj.achannels), acodec(obj.acodec), abitrate(obj.abitrate), asamplerate(obj.asamplerate), aextra(obj.aextra), profiletype(obj.profiletype), deinterlace(obj.deinterlace) {
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
        deinterlace = obj.deinterlace;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Profile::FilterHandle Profile::filter() {
    return Profile::FilterHandle(*this);
}
Profile::GroupHandle Profile::group() {
    return Profile::GroupHandle(*this);
}
Profile::ParamsHandle Profile::params() {
    return Profile::ParamsHandle(*this);
}
Profile::ProjectHandle Profile::project() {
    return Profile::ProjectHandle(*this);
}
Profile::VpresetHandle Profile::vpreset() {
    return Profile::VpresetHandle(*this);
}
Profile::ApresetHandle Profile::apreset() {
    return Profile::ApresetHandle(*this);
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
    fields.push_back(deinterlace.name());
    values.push_back(deinterlace);
    deinterlace.setModified(false);
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
    updateField(updates, table__, deinterlace);
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
    ftypes.push_back(Deinterlace);
}
void Profile::delRecord() {
    deleteFromTable(table__, id);
}
void Profile::delRelations() {
    FilterProfileRelation::del(*db, (FilterProfileRelation::Profile == id));
    ProfileGroup2Profile::del(*db, (ProfileGroup2Profile::Profile == id));
    ProfileProfileParameterRelation::del(*db, (ProfileProfileParameterRelation::Profile == id));
    ProfileProjectRelation::del(*db, (ProfileProjectRelation::Profile == id));
    CodecPresetProfileRelationVideoCodecPreset2Profile::del(*db, (CodecPresetProfileRelationVideoCodecPreset2Profile::Profile == id));
    CodecPresetProfileRelationAudioCodecPreset2Profile::del(*db, (CodecPresetProfileRelationAudioCodecPreset2Profile::Profile == id));
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
    if (!typeIsCorrect()) {
        std::auto_ptr<Profile> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Profile::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<Profile> Profile::upcast() const {
    return auto_ptr<Profile>(new Profile(*this));
}
std::auto_ptr<Profile> Profile::upcastCopy() const {
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
    np->deinterlace = deinterlace;
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
    os << o.deinterlace.name() << " = " << o.deinterlace << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Preset::Own::Id("id_",A_field_type_integer,"Preset_");
Preset::ProjectHandle::ProjectHandle(const Preset& owner)
         : litesql::RelationHandle<Preset>(owner) {
}
void Preset::ProjectHandle::link(const Project& o0) {
    PresetProjectRelation::link(owner->getDatabase(), *owner, o0);
}
void Preset::ProjectHandle::unlink(const Project& o0) {
    PresetProjectRelation::unlink(owner->getDatabase(), *owner, o0);
}
void Preset::ProjectHandle::del(const litesql::Expr& expr) {
    PresetProjectRelation::del(owner->getDatabase(), expr && PresetProjectRelation::Preset == owner->id);
}
litesql::DataSource<Project> Preset::ProjectHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return PresetProjectRelation::get<Project>(owner->getDatabase(), expr, (PresetProjectRelation::Preset == owner->id) && srcExpr);
}
litesql::DataSource<PresetProjectRelation::Row> Preset::ProjectHandle::getRows(const litesql::Expr& expr) {
    return PresetProjectRelation::getRows(owner->getDatabase(), expr && (PresetProjectRelation::Preset == owner->id));
}
Preset::UserHandle::UserHandle(const Preset& owner)
         : litesql::RelationHandle<Preset>(owner) {
}
void Preset::UserHandle::link(const User& o0) {
    PresetUserRelation::link(owner->getDatabase(), *owner, o0);
}
void Preset::UserHandle::unlink(const User& o0) {
    PresetUserRelation::unlink(owner->getDatabase(), *owner, o0);
}
void Preset::UserHandle::del(const litesql::Expr& expr) {
    PresetUserRelation::del(owner->getDatabase(), expr && PresetUserRelation::Preset == owner->id);
}
litesql::DataSource<User> Preset::UserHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return PresetUserRelation::get<User>(owner->getDatabase(), expr, (PresetUserRelation::Preset == owner->id) && srcExpr);
}
litesql::DataSource<PresetUserRelation::Row> Preset::UserHandle::getRows(const litesql::Expr& expr) {
    return PresetUserRelation::getRows(owner->getDatabase(), expr && (PresetUserRelation::Preset == owner->id));
}
Preset::JobHandle::JobHandle(const Preset& owner)
         : litesql::RelationHandle<Preset>(owner) {
}
void Preset::JobHandle::link(const Job& o0) {
    JobPresetRelation::link(owner->getDatabase(), o0, *owner);
}
void Preset::JobHandle::unlink(const Job& o0) {
    JobPresetRelation::unlink(owner->getDatabase(), o0, *owner);
}
void Preset::JobHandle::del(const litesql::Expr& expr) {
    JobPresetRelation::del(owner->getDatabase(), expr && JobPresetRelation::Preset == owner->id);
}
litesql::DataSource<Job> Preset::JobHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobPresetRelation::get<Job>(owner->getDatabase(), expr, (JobPresetRelation::Preset == owner->id) && srcExpr);
}
litesql::DataSource<JobPresetRelation::Row> Preset::JobHandle::getRows(const litesql::Expr& expr) {
    return JobPresetRelation::getRows(owner->getDatabase(), expr && (JobPresetRelation::Preset == owner->id));
}
const std::string Preset::type__("Preset");
const std::string Preset::table__("Preset_");
const std::string Preset::sequence__("Preset_seq");
const litesql::FieldType Preset::Id("id_",A_field_type_integer,table__);
const litesql::FieldType Preset::Type("type_",A_field_type_string,table__);
const litesql::FieldType Preset::Uuid("uuid_",A_field_type_string,table__);
const litesql::FieldType Preset::Name("name_",A_field_type_string,table__);
const litesql::FieldType Preset::Filename("filename_",A_field_type_string,table__);
const litesql::FieldType Preset::Data("data_",A_field_type_string,table__);
void Preset::defaults() {
    id = 0;
}
Preset::Preset(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), uuid(Uuid), name(Name), filename(Filename), data(Data) {
    defaults();
}
Preset::Preset(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), uuid(Uuid), name(Name), filename(Filename), data(Data) {
    defaults();
    size_t size = (rec.size() > 6) ? 6 : rec.size();
    switch(size) {
    case 6: data = convert<const std::string&, std::string>(rec[5]);
        data.setModified(false);
    case 5: filename = convert<const std::string&, std::string>(rec[4]);
        filename.setModified(false);
    case 4: name = convert<const std::string&, std::string>(rec[3]);
        name.setModified(false);
    case 3: uuid = convert<const std::string&, std::string>(rec[2]);
        uuid.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Preset::Preset(const Preset& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), uuid(obj.uuid), name(obj.name), filename(obj.filename), data(obj.data) {
}
const Preset& Preset::operator=(const Preset& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        uuid = obj.uuid;
        name = obj.name;
        filename = obj.filename;
        data = obj.data;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Preset::ProjectHandle Preset::project() {
    return Preset::ProjectHandle(*this);
}
Preset::UserHandle Preset::user() {
    return Preset::UserHandle(*this);
}
Preset::JobHandle Preset::job() {
    return Preset::JobHandle(*this);
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
    fields.push_back(uuid.name());
    values.push_back(uuid);
    uuid.setModified(false);
    fields.push_back(name.name());
    values.push_back(name);
    name.setModified(false);
    fields.push_back(filename.name());
    values.push_back(filename);
    filename.setModified(false);
    fields.push_back(data.name());
    values.push_back(data);
    data.setModified(false);
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
    updateField(updates, table__, uuid);
    updateField(updates, table__, name);
    updateField(updates, table__, filename);
    updateField(updates, table__, data);
}
void Preset::addIDUpdates(Updates& updates) {
}
void Preset::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Uuid);
    ftypes.push_back(Name);
    ftypes.push_back(Filename);
    ftypes.push_back(Data);
}
void Preset::delRecord() {
    deleteFromTable(table__, id);
}
void Preset::delRelations() {
    PresetProjectRelation::del(*db, (PresetProjectRelation::Preset == id));
    PresetUserRelation::del(*db, (PresetUserRelation::Preset == id));
    JobPresetRelation::del(*db, (JobPresetRelation::Preset == id));
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
    if (!typeIsCorrect()) {
        std::auto_ptr<Preset> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Preset::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<Preset> Preset::upcast() const {
    return auto_ptr<Preset>(new Preset(*this));
}
std::auto_ptr<Preset> Preset::upcastCopy() const {
    Preset* np = new Preset(*this);
    np->id = id;
    np->type = type;
    np->uuid = uuid;
    np->name = name;
    np->filename = filename;
    np->data = data;
    np->inDatabase = inDatabase;
    return auto_ptr<Preset>(np);
}
std::ostream & operator<<(std::ostream& os, Preset o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.uuid.name() << " = " << o.uuid << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.filename.name() << " = " << o.filename << std::endl;
    os << o.data.name() << " = " << o.data << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType ProfileParameter::Own::Id("id_",A_field_type_integer,"ProfileParameter_");
ProfileParameter::ProfileHandle::ProfileHandle(const ProfileParameter& owner)
         : litesql::RelationHandle<ProfileParameter>(owner) {
}
void ProfileParameter::ProfileHandle::link(const Profile& o0) {
    ProfileProfileParameterRelation::link(owner->getDatabase(), o0, *owner);
}
void ProfileParameter::ProfileHandle::unlink(const Profile& o0) {
    ProfileProfileParameterRelation::unlink(owner->getDatabase(), o0, *owner);
}
void ProfileParameter::ProfileHandle::del(const litesql::Expr& expr) {
    ProfileProfileParameterRelation::del(owner->getDatabase(), expr && ProfileProfileParameterRelation::ProfileParameter == owner->id);
}
litesql::DataSource<Profile> ProfileParameter::ProfileHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return ProfileProfileParameterRelation::get<Profile>(owner->getDatabase(), expr, (ProfileProfileParameterRelation::ProfileParameter == owner->id) && srcExpr);
}
litesql::DataSource<ProfileProfileParameterRelation::Row> ProfileParameter::ProfileHandle::getRows(const litesql::Expr& expr) {
    return ProfileProfileParameterRelation::getRows(owner->getDatabase(), expr && (ProfileProfileParameterRelation::ProfileParameter == owner->id));
}
const std::string ProfileParameter::type__("ProfileParameter");
const std::string ProfileParameter::table__("ProfileParameter_");
const std::string ProfileParameter::sequence__("ProfileParameter_seq");
const litesql::FieldType ProfileParameter::Id("id_",A_field_type_integer,table__);
const litesql::FieldType ProfileParameter::Type("type_",A_field_type_string,table__);
const litesql::FieldType ProfileParameter::Name("name_",A_field_type_string,table__);
const litesql::FieldType ProfileParameter::Val("val_",A_field_type_string,table__);
const litesql::FieldType ProfileParameter::Mediatype("mediatype_",A_field_type_integer,table__);
void ProfileParameter::defaults() {
    id = 0;
    mediatype = 0;
}
ProfileParameter::ProfileParameter(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name), val(Val), mediatype(Mediatype) {
    defaults();
}
ProfileParameter::ProfileParameter(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name), val(Val), mediatype(Mediatype) {
    defaults();
    size_t size = (rec.size() > 5) ? 5 : rec.size();
    switch(size) {
    case 5: mediatype = convert<const std::string&, int>(rec[4]);
        mediatype.setModified(false);
    case 4: val = convert<const std::string&, std::string>(rec[3]);
        val.setModified(false);
    case 3: name = convert<const std::string&, std::string>(rec[2]);
        name.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
ProfileParameter::ProfileParameter(const ProfileParameter& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name), val(obj.val), mediatype(obj.mediatype) {
}
const ProfileParameter& ProfileParameter::operator=(const ProfileParameter& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
        val = obj.val;
        mediatype = obj.mediatype;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
ProfileParameter::ProfileHandle ProfileParameter::profile() {
    return ProfileParameter::ProfileHandle(*this);
}
std::string ProfileParameter::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
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
    fields.push_back(val.name());
    values.push_back(val);
    val.setModified(false);
    fields.push_back(mediatype.name());
    values.push_back(mediatype);
    mediatype.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void ProfileParameter::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void ProfileParameter::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, name);
    updateField(updates, table__, val);
    updateField(updates, table__, mediatype);
}
void ProfileParameter::addIDUpdates(Updates& updates) {
}
void ProfileParameter::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
    ftypes.push_back(Val);
    ftypes.push_back(Mediatype);
}
void ProfileParameter::delRecord() {
    deleteFromTable(table__, id);
}
void ProfileParameter::delRelations() {
    ProfileProfileParameterRelation::del(*db, (ProfileProfileParameterRelation::ProfileParameter == id));
}
void ProfileParameter::update() {
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
void ProfileParameter::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<ProfileParameter> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool ProfileParameter::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<ProfileParameter> ProfileParameter::upcast() const {
    return auto_ptr<ProfileParameter>(new ProfileParameter(*this));
}
std::auto_ptr<ProfileParameter> ProfileParameter::upcastCopy() const {
    ProfileParameter* np = new ProfileParameter(*this);
    np->id = id;
    np->type = type;
    np->name = name;
    np->val = val;
    np->mediatype = mediatype;
    np->inDatabase = inDatabase;
    return auto_ptr<ProfileParameter>(np);
}
std::ostream & operator<<(std::ostream& os, ProfileParameter o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.val.name() << " = " << o.val << std::endl;
    os << o.mediatype.name() << " = " << o.mediatype << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Stream::Own::Id("id_",A_field_type_integer,"Stream_");
Stream::ParamsHandle::ParamsHandle(const Stream& owner)
         : litesql::RelationHandle<Stream>(owner) {
}
void Stream::ParamsHandle::link(const StreamParameter& o0) {
    StreamStreamParameterRelation::link(owner->getDatabase(), *owner, o0);
}
void Stream::ParamsHandle::unlink(const StreamParameter& o0) {
    StreamStreamParameterRelation::unlink(owner->getDatabase(), *owner, o0);
}
void Stream::ParamsHandle::del(const litesql::Expr& expr) {
    StreamStreamParameterRelation::del(owner->getDatabase(), expr && StreamStreamParameterRelation::Stream == owner->id);
}
litesql::DataSource<StreamParameter> Stream::ParamsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return StreamStreamParameterRelation::get<StreamParameter>(owner->getDatabase(), expr, (StreamStreamParameterRelation::Stream == owner->id) && srcExpr);
}
litesql::DataSource<StreamStreamParameterRelation::Row> Stream::ParamsHandle::getRows(const litesql::Expr& expr) {
    return StreamStreamParameterRelation::getRows(owner->getDatabase(), expr && (StreamStreamParameterRelation::Stream == owner->id));
}
Stream::FiltersHandle::FiltersHandle(const Stream& owner)
         : litesql::RelationHandle<Stream>(owner) {
}
void Stream::FiltersHandle::link(const Filter& o0) {
    FilterStreamRelation::link(owner->getDatabase(), o0, *owner);
}
void Stream::FiltersHandle::unlink(const Filter& o0) {
    FilterStreamRelation::unlink(owner->getDatabase(), o0, *owner);
}
void Stream::FiltersHandle::del(const litesql::Expr& expr) {
    FilterStreamRelation::del(owner->getDatabase(), expr && FilterStreamRelation::Stream == owner->id);
}
litesql::DataSource<Filter> Stream::FiltersHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return FilterStreamRelation::get<Filter>(owner->getDatabase(), expr, (FilterStreamRelation::Stream == owner->id) && srcExpr);
}
litesql::DataSource<FilterStreamRelation::Row> Stream::FiltersHandle::getRows(const litesql::Expr& expr) {
    return FilterStreamRelation::getRows(owner->getDatabase(), expr && (FilterStreamRelation::Stream == owner->id));
}
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
const litesql::FieldType Stream::Id("id_",A_field_type_integer,table__);
const litesql::FieldType Stream::Type("type_",A_field_type_string,table__);
const litesql::FieldType Stream::Streamindex("streamindex_",A_field_type_integer,table__);
const litesql::FieldType Stream::Streamtype("streamtype_",A_field_type_integer,table__);
const litesql::FieldType Stream::Codecid("codecid_",A_field_type_integer,table__);
const litesql::FieldType Stream::Codecname("codecname_",A_field_type_string,table__);
const litesql::FieldType Stream::Frameratenum("frameratenum_",A_field_type_integer,table__);
const litesql::FieldType Stream::Framerateden("framerateden_",A_field_type_integer,table__);
const litesql::FieldType Stream::Streamtimebasenum("streamtimebasenum_",A_field_type_integer,table__);
const litesql::FieldType Stream::Streamtimebaseden("streamtimebaseden_",A_field_type_integer,table__);
const litesql::FieldType Stream::Codectimebasenum("codectimebasenum_",A_field_type_integer,table__);
const litesql::FieldType Stream::Codectimebaseden("codectimebaseden_",A_field_type_integer,table__);
const litesql::FieldType Stream::Firstpts("firstpts_",A_field_type_double,table__);
const litesql::FieldType Stream::Firstdts("firstdts_",A_field_type_double,table__);
const litesql::FieldType Stream::Duration("duration_",A_field_type_double,table__);
const litesql::FieldType Stream::Nbframes("nbframes_",A_field_type_double,table__);
const litesql::FieldType Stream::Ticksperframe("ticksperframe_",A_field_type_integer,table__);
const litesql::FieldType Stream::Framecount("framecount_",A_field_type_integer,table__);
const litesql::FieldType Stream::Width("width_",A_field_type_integer,table__);
const litesql::FieldType Stream::Height("height_",A_field_type_integer,table__);
const litesql::FieldType Stream::Gopsize("gopsize_",A_field_type_integer,table__);
const litesql::FieldType Stream::Pixfmt("pixfmt_",A_field_type_integer,table__);
const litesql::FieldType Stream::Bitrate("bitrate_",A_field_type_integer,table__);
const litesql::FieldType Stream::Samplerate("samplerate_",A_field_type_integer,table__);
const litesql::FieldType Stream::Samplefmt("samplefmt_",A_field_type_integer,table__);
const litesql::FieldType Stream::Channels("channels_",A_field_type_integer,table__);
const litesql::FieldType Stream::Bitspercodedsample("bitspercodedsample_",A_field_type_integer,table__);
const litesql::FieldType Stream::Privdatasize("privdatasize_",A_field_type_integer,table__);
const litesql::FieldType Stream::Privdata("privdata_",A_field_type_string,table__);
const litesql::FieldType Stream::Extradatasize("extradatasize_",A_field_type_integer,table__);
const litesql::FieldType Stream::Extradata("extradata_",A_field_type_blob,table__);
const litesql::FieldType Stream::Aspectratio("aspectratio_",A_field_type_string,table__);
const litesql::FieldType Stream::Flags("flags_",A_field_type_integer,table__);
const litesql::FieldType Stream::Extraprofileflags("extraprofileflags_",A_field_type_string,table__);
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
    extradata = Blob::nil;
    flags = 0;
}
Stream::Stream(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), streamindex(Streamindex), streamtype(Streamtype), codecid(Codecid), codecname(Codecname), frameratenum(Frameratenum), framerateden(Framerateden), streamtimebasenum(Streamtimebasenum), streamtimebaseden(Streamtimebaseden), codectimebasenum(Codectimebasenum), codectimebaseden(Codectimebaseden), firstpts(Firstpts), firstdts(Firstdts), duration(Duration), nbframes(Nbframes), ticksperframe(Ticksperframe), framecount(Framecount), width(Width), height(Height), gopsize(Gopsize), pixfmt(Pixfmt), bitrate(Bitrate), samplerate(Samplerate), samplefmt(Samplefmt), channels(Channels), bitspercodedsample(Bitspercodedsample), privdatasize(Privdatasize), privdata(Privdata), extradatasize(Extradatasize), extradata(Extradata), aspectratio(Aspectratio), flags(Flags), extraprofileflags(Extraprofileflags) {
    defaults();
}
Stream::Stream(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), streamindex(Streamindex), streamtype(Streamtype), codecid(Codecid), codecname(Codecname), frameratenum(Frameratenum), framerateden(Framerateden), streamtimebasenum(Streamtimebasenum), streamtimebaseden(Streamtimebaseden), codectimebasenum(Codectimebasenum), codectimebaseden(Codectimebaseden), firstpts(Firstpts), firstdts(Firstdts), duration(Duration), nbframes(Nbframes), ticksperframe(Ticksperframe), framecount(Framecount), width(Width), height(Height), gopsize(Gopsize), pixfmt(Pixfmt), bitrate(Bitrate), samplerate(Samplerate), samplefmt(Samplefmt), channels(Channels), bitspercodedsample(Bitspercodedsample), privdatasize(Privdatasize), privdata(Privdata), extradatasize(Extradatasize), extradata(Extradata), aspectratio(Aspectratio), flags(Flags), extraprofileflags(Extraprofileflags) {
    defaults();
    size_t size = (rec.size() > 34) ? 34 : rec.size();
    switch(size) {
    case 34: extraprofileflags = convert<const std::string&, std::string>(rec[33]);
        extraprofileflags.setModified(false);
    case 33: flags = convert<const std::string&, int>(rec[32]);
        flags.setModified(false);
    case 32: aspectratio = convert<const std::string&, std::string>(rec[31]);
        aspectratio.setModified(false);
    case 31: extradata = convert<const std::string&, litesql::Blob>(rec[30]);
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
     : litesql::Persistent(obj), id(obj.id), type(obj.type), streamindex(obj.streamindex), streamtype(obj.streamtype), codecid(obj.codecid), codecname(obj.codecname), frameratenum(obj.frameratenum), framerateden(obj.framerateden), streamtimebasenum(obj.streamtimebasenum), streamtimebaseden(obj.streamtimebaseden), codectimebasenum(obj.codectimebasenum), codectimebaseden(obj.codectimebaseden), firstpts(obj.firstpts), firstdts(obj.firstdts), duration(obj.duration), nbframes(obj.nbframes), ticksperframe(obj.ticksperframe), framecount(obj.framecount), width(obj.width), height(obj.height), gopsize(obj.gopsize), pixfmt(obj.pixfmt), bitrate(obj.bitrate), samplerate(obj.samplerate), samplefmt(obj.samplefmt), channels(obj.channels), bitspercodedsample(obj.bitspercodedsample), privdatasize(obj.privdatasize), privdata(obj.privdata), extradatasize(obj.extradatasize), extradata(obj.extradata), aspectratio(obj.aspectratio), flags(obj.flags), extraprofileflags(obj.extraprofileflags) {
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
        aspectratio = obj.aspectratio;
        flags = obj.flags;
        extraprofileflags = obj.extraprofileflags;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Stream::ParamsHandle Stream::params() {
    return Stream::ParamsHandle(*this);
}
Stream::FiltersHandle Stream::filters() {
    return Stream::FiltersHandle(*this);
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
    fields.push_back(aspectratio.name());
    values.push_back(aspectratio);
    aspectratio.setModified(false);
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
    updateField(updates, table__, aspectratio);
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
    ftypes.push_back(Aspectratio);
    ftypes.push_back(Flags);
    ftypes.push_back(Extraprofileflags);
}
void Stream::delRecord() {
    deleteFromTable(table__, id);
}
void Stream::delRelations() {
    StreamStreamParameterRelation::del(*db, (StreamStreamParameterRelation::Stream == id));
    FilterStreamRelation::del(*db, (FilterStreamRelation::Stream == id));
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
    if (!typeIsCorrect()) {
        std::auto_ptr<Stream> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Stream::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<Stream> Stream::upcast() const {
    return auto_ptr<Stream>(new Stream(*this));
}
std::auto_ptr<Stream> Stream::upcastCopy() const {
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
    np->aspectratio = aspectratio;
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
    os << o.aspectratio.name() << " = " << o.aspectratio << std::endl;
    os << o.flags.name() << " = " << o.flags << std::endl;
    os << o.extraprofileflags.name() << " = " << o.extraprofileflags << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType StreamParameter::Own::Id("id_",A_field_type_integer,"StreamParameter_");
StreamParameter::StreamHandle::StreamHandle(const StreamParameter& owner)
         : litesql::RelationHandle<StreamParameter>(owner) {
}
void StreamParameter::StreamHandle::link(const Stream& o0) {
    StreamStreamParameterRelation::link(owner->getDatabase(), o0, *owner);
}
void StreamParameter::StreamHandle::unlink(const Stream& o0) {
    StreamStreamParameterRelation::unlink(owner->getDatabase(), o0, *owner);
}
void StreamParameter::StreamHandle::del(const litesql::Expr& expr) {
    StreamStreamParameterRelation::del(owner->getDatabase(), expr && StreamStreamParameterRelation::StreamParameter == owner->id);
}
litesql::DataSource<Stream> StreamParameter::StreamHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return StreamStreamParameterRelation::get<Stream>(owner->getDatabase(), expr, (StreamStreamParameterRelation::StreamParameter == owner->id) && srcExpr);
}
litesql::DataSource<StreamStreamParameterRelation::Row> StreamParameter::StreamHandle::getRows(const litesql::Expr& expr) {
    return StreamStreamParameterRelation::getRows(owner->getDatabase(), expr && (StreamStreamParameterRelation::StreamParameter == owner->id));
}
const std::string StreamParameter::type__("StreamParameter");
const std::string StreamParameter::table__("StreamParameter_");
const std::string StreamParameter::sequence__("StreamParameter_seq");
const litesql::FieldType StreamParameter::Id("id_",A_field_type_integer,table__);
const litesql::FieldType StreamParameter::Type("type_",A_field_type_string,table__);
const litesql::FieldType StreamParameter::Name("name_",A_field_type_string,table__);
const litesql::FieldType StreamParameter::Val("val_",A_field_type_string,table__);
void StreamParameter::defaults() {
    id = 0;
}
StreamParameter::StreamParameter(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name), val(Val) {
    defaults();
}
StreamParameter::StreamParameter(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name), val(Val) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: val = convert<const std::string&, std::string>(rec[3]);
        val.setModified(false);
    case 3: name = convert<const std::string&, std::string>(rec[2]);
        name.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
StreamParameter::StreamParameter(const StreamParameter& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name), val(obj.val) {
}
const StreamParameter& StreamParameter::operator=(const StreamParameter& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
        val = obj.val;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
StreamParameter::StreamHandle StreamParameter::stream() {
    return StreamParameter::StreamHandle(*this);
}
std::string StreamParameter::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
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
    fields.push_back(val.name());
    values.push_back(val);
    val.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void StreamParameter::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void StreamParameter::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, name);
    updateField(updates, table__, val);
}
void StreamParameter::addIDUpdates(Updates& updates) {
}
void StreamParameter::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
    ftypes.push_back(Val);
}
void StreamParameter::delRecord() {
    deleteFromTable(table__, id);
}
void StreamParameter::delRelations() {
    StreamStreamParameterRelation::del(*db, (StreamStreamParameterRelation::StreamParameter == id));
}
void StreamParameter::update() {
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
void StreamParameter::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<StreamParameter> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool StreamParameter::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<StreamParameter> StreamParameter::upcast() const {
    return auto_ptr<StreamParameter>(new StreamParameter(*this));
}
std::auto_ptr<StreamParameter> StreamParameter::upcastCopy() const {
    StreamParameter* np = new StreamParameter(*this);
    np->id = id;
    np->type = type;
    np->name = name;
    np->val = val;
    np->inDatabase = inDatabase;
    return auto_ptr<StreamParameter>(np);
}
std::ostream & operator<<(std::ostream& os, StreamParameter o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.val.name() << " = " << o.val << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType CodecPreset::Own::Id("id_",A_field_type_integer,"CodecPreset_");
CodecPreset::ParamsHandle::ParamsHandle(const CodecPreset& owner)
         : litesql::RelationHandle<CodecPreset>(owner) {
}
void CodecPreset::ParamsHandle::link(const CodecPresetParameter& o0) {
    CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::link(owner->getDatabase(), *owner, o0);
}
void CodecPreset::ParamsHandle::unlink(const CodecPresetParameter& o0) {
    CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::unlink(owner->getDatabase(), *owner, o0);
}
void CodecPreset::ParamsHandle::del(const litesql::Expr& expr) {
    CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::del(owner->getDatabase(), expr && CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::CodecPreset == owner->id);
}
litesql::DataSource<CodecPresetParameter> CodecPreset::ParamsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::get<CodecPresetParameter>(owner->getDatabase(), expr, (CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::CodecPreset == owner->id) && srcExpr);
}
litesql::DataSource<CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::Row> CodecPreset::ParamsHandle::getRows(const litesql::Expr& expr) {
    return CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::getRows(owner->getDatabase(), expr && (CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::CodecPreset == owner->id));
}
CodecPreset::VprofileHandle::VprofileHandle(const CodecPreset& owner)
         : litesql::RelationHandle<CodecPreset>(owner) {
}
void CodecPreset::VprofileHandle::link(const Profile& o0) {
    CodecPresetProfileRelationVideoCodecPreset2Profile::link(owner->getDatabase(), *owner, o0);
}
void CodecPreset::VprofileHandle::unlink(const Profile& o0) {
    CodecPresetProfileRelationVideoCodecPreset2Profile::unlink(owner->getDatabase(), *owner, o0);
}
void CodecPreset::VprofileHandle::del(const litesql::Expr& expr) {
    CodecPresetProfileRelationVideoCodecPreset2Profile::del(owner->getDatabase(), expr && CodecPresetProfileRelationVideoCodecPreset2Profile::CodecPreset == owner->id);
}
litesql::DataSource<Profile> CodecPreset::VprofileHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return CodecPresetProfileRelationVideoCodecPreset2Profile::get<Profile>(owner->getDatabase(), expr, (CodecPresetProfileRelationVideoCodecPreset2Profile::CodecPreset == owner->id) && srcExpr);
}
litesql::DataSource<CodecPresetProfileRelationVideoCodecPreset2Profile::Row> CodecPreset::VprofileHandle::getRows(const litesql::Expr& expr) {
    return CodecPresetProfileRelationVideoCodecPreset2Profile::getRows(owner->getDatabase(), expr && (CodecPresetProfileRelationVideoCodecPreset2Profile::CodecPreset == owner->id));
}
CodecPreset::AprofileHandle::AprofileHandle(const CodecPreset& owner)
         : litesql::RelationHandle<CodecPreset>(owner) {
}
void CodecPreset::AprofileHandle::link(const Profile& o0) {
    CodecPresetProfileRelationAudioCodecPreset2Profile::link(owner->getDatabase(), *owner, o0);
}
void CodecPreset::AprofileHandle::unlink(const Profile& o0) {
    CodecPresetProfileRelationAudioCodecPreset2Profile::unlink(owner->getDatabase(), *owner, o0);
}
void CodecPreset::AprofileHandle::del(const litesql::Expr& expr) {
    CodecPresetProfileRelationAudioCodecPreset2Profile::del(owner->getDatabase(), expr && CodecPresetProfileRelationAudioCodecPreset2Profile::CodecPreset == owner->id);
}
litesql::DataSource<Profile> CodecPreset::AprofileHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return CodecPresetProfileRelationAudioCodecPreset2Profile::get<Profile>(owner->getDatabase(), expr, (CodecPresetProfileRelationAudioCodecPreset2Profile::CodecPreset == owner->id) && srcExpr);
}
litesql::DataSource<CodecPresetProfileRelationAudioCodecPreset2Profile::Row> CodecPreset::AprofileHandle::getRows(const litesql::Expr& expr) {
    return CodecPresetProfileRelationAudioCodecPreset2Profile::getRows(owner->getDatabase(), expr && (CodecPresetProfileRelationAudioCodecPreset2Profile::CodecPreset == owner->id));
}
const std::string CodecPreset::type__("CodecPreset");
const std::string CodecPreset::table__("CodecPreset_");
const std::string CodecPreset::sequence__("CodecPreset_seq");
const litesql::FieldType CodecPreset::Id("id_",A_field_type_integer,table__);
const litesql::FieldType CodecPreset::Type("type_",A_field_type_string,table__);
const litesql::FieldType CodecPreset::Name("name_",A_field_type_string,table__);
const litesql::FieldType CodecPreset::Created("created_",A_field_type_date,table__);
const litesql::FieldType CodecPreset::Codecid("codecid_",A_field_type_integer,table__);
const litesql::FieldType CodecPreset::Preset("preset_",A_field_type_string,table__);
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
CodecPreset::ParamsHandle CodecPreset::params() {
    return CodecPreset::ParamsHandle(*this);
}
CodecPreset::VprofileHandle CodecPreset::vprofile() {
    return CodecPreset::VprofileHandle(*this);
}
CodecPreset::AprofileHandle CodecPreset::aprofile() {
    return CodecPreset::AprofileHandle(*this);
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
    CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::del(*db, (CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::CodecPreset == id));
    CodecPresetProfileRelationVideoCodecPreset2Profile::del(*db, (CodecPresetProfileRelationVideoCodecPreset2Profile::CodecPreset == id));
    CodecPresetProfileRelationAudioCodecPreset2Profile::del(*db, (CodecPresetProfileRelationAudioCodecPreset2Profile::CodecPreset == id));
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
    if (!typeIsCorrect()) {
        std::auto_ptr<CodecPreset> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool CodecPreset::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<CodecPreset> CodecPreset::upcast() const {
    return auto_ptr<CodecPreset>(new CodecPreset(*this));
}
std::auto_ptr<CodecPreset> CodecPreset::upcastCopy() const {
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
const litesql::FieldType CodecPresetParameter::Own::Id("id_",A_field_type_integer,"CodecPresetParameter_");
CodecPresetParameter::PresetHandle::PresetHandle(const CodecPresetParameter& owner)
         : litesql::RelationHandle<CodecPresetParameter>(owner) {
}
void CodecPresetParameter::PresetHandle::link(const CodecPreset& o0) {
    CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::link(owner->getDatabase(), o0, *owner);
}
void CodecPresetParameter::PresetHandle::unlink(const CodecPreset& o0) {
    CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::unlink(owner->getDatabase(), o0, *owner);
}
void CodecPresetParameter::PresetHandle::del(const litesql::Expr& expr) {
    CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::del(owner->getDatabase(), expr && CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::CodecPresetParameter == owner->id);
}
litesql::DataSource<CodecPreset> CodecPresetParameter::PresetHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::get<CodecPreset>(owner->getDatabase(), expr, (CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::CodecPresetParameter == owner->id) && srcExpr);
}
litesql::DataSource<CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::Row> CodecPresetParameter::PresetHandle::getRows(const litesql::Expr& expr) {
    return CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::getRows(owner->getDatabase(), expr && (CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::CodecPresetParameter == owner->id));
}
const std::string CodecPresetParameter::type__("CodecPresetParameter");
const std::string CodecPresetParameter::table__("CodecPresetParameter_");
const std::string CodecPresetParameter::sequence__("CodecPresetParameter_seq");
const litesql::FieldType CodecPresetParameter::Id("id_",A_field_type_integer,table__);
const litesql::FieldType CodecPresetParameter::Type("type_",A_field_type_string,table__);
const litesql::FieldType CodecPresetParameter::Name("name_",A_field_type_string,table__);
const litesql::FieldType CodecPresetParameter::Val("val_",A_field_type_string,table__);
void CodecPresetParameter::defaults() {
    id = 0;
}
CodecPresetParameter::CodecPresetParameter(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name), val(Val) {
    defaults();
}
CodecPresetParameter::CodecPresetParameter(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name), val(Val) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: val = convert<const std::string&, std::string>(rec[3]);
        val.setModified(false);
    case 3: name = convert<const std::string&, std::string>(rec[2]);
        name.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
CodecPresetParameter::CodecPresetParameter(const CodecPresetParameter& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name), val(obj.val) {
}
const CodecPresetParameter& CodecPresetParameter::operator=(const CodecPresetParameter& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
        val = obj.val;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
CodecPresetParameter::PresetHandle CodecPresetParameter::preset() {
    return CodecPresetParameter::PresetHandle(*this);
}
std::string CodecPresetParameter::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
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
    fields.push_back(val.name());
    values.push_back(val);
    val.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void CodecPresetParameter::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void CodecPresetParameter::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, name);
    updateField(updates, table__, val);
}
void CodecPresetParameter::addIDUpdates(Updates& updates) {
}
void CodecPresetParameter::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
    ftypes.push_back(Val);
}
void CodecPresetParameter::delRecord() {
    deleteFromTable(table__, id);
}
void CodecPresetParameter::delRelations() {
    CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::del(*db, (CodecPresetCodecPresetParameterRelationCodecPreset2Parameter::CodecPresetParameter == id));
}
void CodecPresetParameter::update() {
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
void CodecPresetParameter::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<CodecPresetParameter> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool CodecPresetParameter::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<CodecPresetParameter> CodecPresetParameter::upcast() const {
    return auto_ptr<CodecPresetParameter>(new CodecPresetParameter(*this));
}
std::auto_ptr<CodecPresetParameter> CodecPresetParameter::upcastCopy() const {
    CodecPresetParameter* np = new CodecPresetParameter(*this);
    np->id = id;
    np->type = type;
    np->name = name;
    np->val = val;
    np->inDatabase = inDatabase;
    return auto_ptr<CodecPresetParameter>(np);
}
std::ostream & operator<<(std::ostream& os, CodecPresetParameter o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.val.name() << " = " << o.val << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Config::Own::Id("id_",A_field_type_integer,"Config_");
const std::string Config::type__("Config");
const std::string Config::table__("Config_");
const std::string Config::sequence__("Config_seq");
const litesql::FieldType Config::Id("id_",A_field_type_integer,table__);
const litesql::FieldType Config::Type("type_",A_field_type_string,table__);
const litesql::FieldType Config::Configkey("configkey_",A_field_type_string,table__);
const litesql::FieldType Config::Configval("configval_",A_field_type_string,table__);
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
    if (!typeIsCorrect()) {
        std::auto_ptr<Config> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Config::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<Config> Config::upcast() const {
    return auto_ptr<Config>(new Config(*this));
}
std::auto_ptr<Config> Config::upcastCopy() const {
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
const litesql::FieldType Job::Own::Id("id_",A_field_type_integer,"Job_");
const std::string Job::StatusType::Waiting("WAITING");
const std::string Job::StatusType::Processing("PROCESSING");
const std::string Job::StatusType::Error("ERROR");
const std::string Job::StatusType::Stopping("STOPPING");
const std::string Job::StatusType::Stopped("STOPPED");
const std::string Job::StatusType::Exporting("EXPORTING");
const std::string Job::StatusType::Completed("COMPLETED");
const std::string Job::StatusType::Deleted("DELETED");
const std::string Job::StatusType::CompletedWithError("COMPLETEDWITHERROR");
Job::StatusType::StatusType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals)
         : litesql::FieldType(n,t,tbl,vals) {
}
const std::string Job::Status::Waiting("WAITING");
const std::string Job::Status::Processing("PROCESSING");
const std::string Job::Status::Error("ERROR");
const std::string Job::Status::Stopping("STOPPING");
const std::string Job::Status::Stopped("STOPPED");
const std::string Job::Status::Exporting("EXPORTING");
const std::string Job::Status::Completed("COMPLETED");
const std::string Job::Status::Deleted("DELETED");
const std::string Job::Status::CompletedWithError("COMPLETEDWITHERROR");
Job::TasksHandle::TasksHandle(const Job& owner)
         : litesql::RelationHandle<Job>(owner) {
}
void Job::TasksHandle::link(const Task& o0) {
    JobTaskRelationJobTask::link(owner->getDatabase(), *owner, o0);
}
void Job::TasksHandle::unlink(const Task& o0) {
    JobTaskRelationJobTask::unlink(owner->getDatabase(), *owner, o0);
}
void Job::TasksHandle::del(const litesql::Expr& expr) {
    JobTaskRelationJobTask::del(owner->getDatabase(), expr && JobTaskRelationJobTask::Job == owner->id);
}
litesql::DataSource<Task> Job::TasksHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobTaskRelationJobTask::get<Task>(owner->getDatabase(), expr, (JobTaskRelationJobTask::Job == owner->id) && srcExpr);
}
litesql::DataSource<JobTaskRelationJobTask::Row> Job::TasksHandle::getRows(const litesql::Expr& expr) {
    return JobTaskRelationJobTask::getRows(owner->getDatabase(), expr && (JobTaskRelationJobTask::Job == owner->id));
}
Job::JoblogHandle::JoblogHandle(const Job& owner)
         : litesql::RelationHandle<Job>(owner) {
}
void Job::JoblogHandle::link(const JobLog& o0) {
    JobJobLogRelationJobJobLog::link(owner->getDatabase(), *owner, o0);
}
void Job::JoblogHandle::unlink(const JobLog& o0) {
    JobJobLogRelationJobJobLog::unlink(owner->getDatabase(), *owner, o0);
}
void Job::JoblogHandle::del(const litesql::Expr& expr) {
    JobJobLogRelationJobJobLog::del(owner->getDatabase(), expr && JobJobLogRelationJobJobLog::Job == owner->id);
}
litesql::DataSource<JobLog> Job::JoblogHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobJobLogRelationJobJobLog::get<JobLog>(owner->getDatabase(), expr, (JobJobLogRelationJobJobLog::Job == owner->id) && srcExpr);
}
litesql::DataSource<JobJobLogRelationJobJobLog::Row> Job::JoblogHandle::getRows(const litesql::Expr& expr) {
    return JobJobLogRelationJobJobLog::getRows(owner->getDatabase(), expr && (JobJobLogRelationJobJobLog::Job == owner->id));
}
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
Job::PresetHandle::PresetHandle(const Job& owner)
         : litesql::RelationHandle<Job>(owner) {
}
void Job::PresetHandle::link(const Preset& o0) {
    JobPresetRelation::link(owner->getDatabase(), *owner, o0);
}
void Job::PresetHandle::unlink(const Preset& o0) {
    JobPresetRelation::unlink(owner->getDatabase(), *owner, o0);
}
void Job::PresetHandle::del(const litesql::Expr& expr) {
    JobPresetRelation::del(owner->getDatabase(), expr && JobPresetRelation::Job == owner->id);
}
litesql::DataSource<Preset> Job::PresetHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobPresetRelation::get<Preset>(owner->getDatabase(), expr, (JobPresetRelation::Job == owner->id) && srcExpr);
}
litesql::DataSource<JobPresetRelation::Row> Job::PresetHandle::getRows(const litesql::Expr& expr) {
    return JobPresetRelation::getRows(owner->getDatabase(), expr && (JobPresetRelation::Job == owner->id));
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
Job::ProcessUnitsHandle::ProcessUnitsHandle(const Job& owner)
         : litesql::RelationHandle<Job>(owner) {
}
void Job::ProcessUnitsHandle::link(const ProcessUnit& o0) {
    JobProcessUnitRelationProcessUnitJob::link(owner->getDatabase(), *owner, o0);
}
void Job::ProcessUnitsHandle::unlink(const ProcessUnit& o0) {
    JobProcessUnitRelationProcessUnitJob::unlink(owner->getDatabase(), *owner, o0);
}
void Job::ProcessUnitsHandle::del(const litesql::Expr& expr) {
    JobProcessUnitRelationProcessUnitJob::del(owner->getDatabase(), expr && JobProcessUnitRelationProcessUnitJob::Job == owner->id);
}
litesql::DataSource<ProcessUnit> Job::ProcessUnitsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobProcessUnitRelationProcessUnitJob::get<ProcessUnit>(owner->getDatabase(), expr, (JobProcessUnitRelationProcessUnitJob::Job == owner->id) && srcExpr);
}
litesql::DataSource<JobProcessUnitRelationProcessUnitJob::Row> Job::ProcessUnitsHandle::getRows(const litesql::Expr& expr) {
    return JobProcessUnitRelationProcessUnitJob::getRows(owner->getDatabase(), expr && (JobProcessUnitRelationProcessUnitJob::Job == owner->id));
}
Job::PartitionHandle::PartitionHandle(const Job& owner)
         : litesql::RelationHandle<Job>(owner) {
}
void Job::PartitionHandle::link(const Partition& o0) {
    JobPartitionRelationJob2Partition::link(owner->getDatabase(), *owner, o0);
}
void Job::PartitionHandle::unlink(const Partition& o0) {
    JobPartitionRelationJob2Partition::unlink(owner->getDatabase(), *owner, o0);
}
void Job::PartitionHandle::del(const litesql::Expr& expr) {
    JobPartitionRelationJob2Partition::del(owner->getDatabase(), expr && JobPartitionRelationJob2Partition::Job == owner->id);
}
litesql::DataSource<Partition> Job::PartitionHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobPartitionRelationJob2Partition::get<Partition>(owner->getDatabase(), expr, (JobPartitionRelationJob2Partition::Job == owner->id) && srcExpr);
}
litesql::DataSource<JobPartitionRelationJob2Partition::Row> Job::PartitionHandle::getRows(const litesql::Expr& expr) {
    return JobPartitionRelationJob2Partition::getRows(owner->getDatabase(), expr && (JobPartitionRelationJob2Partition::Job == owner->id));
}
Job::QueuesHandle::QueuesHandle(const Job& owner)
         : litesql::RelationHandle<Job>(owner) {
}
void Job::QueuesHandle::link(const Queue& o0) {
    JobQueueRelationJob2Queue::link(owner->getDatabase(), *owner, o0);
}
void Job::QueuesHandle::unlink(const Queue& o0) {
    JobQueueRelationJob2Queue::unlink(owner->getDatabase(), *owner, o0);
}
void Job::QueuesHandle::del(const litesql::Expr& expr) {
    JobQueueRelationJob2Queue::del(owner->getDatabase(), expr && JobQueueRelationJob2Queue::Job == owner->id);
}
litesql::DataSource<Queue> Job::QueuesHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobQueueRelationJob2Queue::get<Queue>(owner->getDatabase(), expr, (JobQueueRelationJob2Queue::Job == owner->id) && srcExpr);
}
litesql::DataSource<JobQueueRelationJob2Queue::Row> Job::QueuesHandle::getRows(const litesql::Expr& expr) {
    return JobQueueRelationJob2Queue::getRows(owner->getDatabase(), expr && (JobQueueRelationJob2Queue::Job == owner->id));
}
const std::string Job::type__("Job");
const std::string Job::table__("Job_");
const std::string Job::sequence__("Job_seq");
const litesql::FieldType Job::Id("id_",A_field_type_integer,table__);
const litesql::FieldType Job::Type("type_",A_field_type_string,table__);
const litesql::FieldType Job::Uuid("uuid_",A_field_type_string,table__);
const litesql::FieldType Job::Created("created_",A_field_type_datetime,table__);
const litesql::FieldType Job::Begintime("begintime_",A_field_type_datetime,table__);
const litesql::FieldType Job::Endtime("endtime_",A_field_type_datetime,table__);
std::vector < std::pair< std::string, std::string > > Job::status_values;
const Job::StatusType Job::Status("status_",A_field_type_string,table__,status_values);
const Job::StatusType Job::Infile("infile_",A_field_type_string,table__);
const Job::StatusType Job::Outfile("outfile_",A_field_type_string,table__);
const Job::StatusType Job::Starttime("starttime_",A_field_type_double,table__);
const Job::StatusType Job::Duration("duration_",A_field_type_double,table__);
const Job::StatusType Job::Progress("progress_",A_field_type_integer,table__);
const Job::StatusType Job::Fps("fps_",A_field_type_integer,table__);
const Job::StatusType Job::Data("data_",A_field_type_string,table__);
const Job::StatusType Job::Deleted("deleted_",A_field_type_integer,table__);
const Job::StatusType Job::Partitionname("partitionname_",A_field_type_string,table__);
const Job::StatusType Job::Graph("graph_",A_field_type_string,table__);
const Job::StatusType Job::Graphstatus("graphstatus_",A_field_type_string,table__);
const Job::StatusType Job::Graphname("graphname_",A_field_type_string,table__);
void Job::initValues() {
    status_values.clear();
    status_values.push_back(make_pair<std::string, std::string>("Waiting","WAITING"));
    status_values.push_back(make_pair<std::string, std::string>("Processing","PROCESSING"));
    status_values.push_back(make_pair<std::string, std::string>("Error","ERROR"));
    status_values.push_back(make_pair<std::string, std::string>("Stopping","STOPPING"));
    status_values.push_back(make_pair<std::string, std::string>("Stopped","STOPPED"));
    status_values.push_back(make_pair<std::string, std::string>("Exporting","EXPORTING"));
    status_values.push_back(make_pair<std::string, std::string>("Completed","COMPLETED"));
    status_values.push_back(make_pair<std::string, std::string>("Deleted","DELETED"));
    status_values.push_back(make_pair<std::string, std::string>("CompletedWithError","COMPLETEDWITHERROR"));
}
void Job::defaults() {
    id = 0;
    created = -1;
    begintime = -1;
    endtime = -1;
    starttime = 0.0;
    duration = 0.0;
    progress = 0;
    fps = 0;
    deleted = 0;
}
Job::Job(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), uuid(Uuid), created(Created), begintime(Begintime), endtime(Endtime), status(Status), infile(Infile), outfile(Outfile), starttime(Starttime), duration(Duration), progress(Progress), fps(Fps), data(Data), deleted(Deleted), partitionname(Partitionname), graph(Graph), graphstatus(Graphstatus), graphname(Graphname) {
    defaults();
}
Job::Job(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), uuid(Uuid), created(Created), begintime(Begintime), endtime(Endtime), status(Status), infile(Infile), outfile(Outfile), starttime(Starttime), duration(Duration), progress(Progress), fps(Fps), data(Data), deleted(Deleted), partitionname(Partitionname), graph(Graph), graphstatus(Graphstatus), graphname(Graphname) {
    defaults();
    size_t size = (rec.size() > 19) ? 19 : rec.size();
    switch(size) {
    case 19: graphname = convert<const std::string&, std::string>(rec[18]);
        graphname.setModified(false);
    case 18: graphstatus = convert<const std::string&, std::string>(rec[17]);
        graphstatus.setModified(false);
    case 17: graph = convert<const std::string&, std::string>(rec[16]);
        graph.setModified(false);
    case 16: partitionname = convert<const std::string&, std::string>(rec[15]);
        partitionname.setModified(false);
    case 15: deleted = convert<const std::string&, int>(rec[14]);
        deleted.setModified(false);
    case 14: data = convert<const std::string&, std::string>(rec[13]);
        data.setModified(false);
    case 13: fps = convert<const std::string&, int>(rec[12]);
        fps.setModified(false);
    case 12: progress = convert<const std::string&, int>(rec[11]);
        progress.setModified(false);
    case 11: duration = convert<const std::string&, double>(rec[10]);
        duration.setModified(false);
    case 10: starttime = convert<const std::string&, double>(rec[9]);
        starttime.setModified(false);
    case 9: outfile = convert<const std::string&, std::string>(rec[8]);
        outfile.setModified(false);
    case 8: infile = convert<const std::string&, std::string>(rec[7]);
        infile.setModified(false);
    case 7: status = convert<const std::string&, std::string>(rec[6]);
        status.setModified(false);
    case 6: endtime = convert<const std::string&, litesql::DateTime>(rec[5]);
        endtime.setModified(false);
    case 5: begintime = convert<const std::string&, litesql::DateTime>(rec[4]);
        begintime.setModified(false);
    case 4: created = convert<const std::string&, litesql::DateTime>(rec[3]);
        created.setModified(false);
    case 3: uuid = convert<const std::string&, std::string>(rec[2]);
        uuid.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Job::Job(const Job& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), uuid(obj.uuid), created(obj.created), begintime(obj.begintime), endtime(obj.endtime), status(obj.status), infile(obj.infile), outfile(obj.outfile), starttime(obj.starttime), duration(obj.duration), progress(obj.progress), fps(obj.fps), data(obj.data), deleted(obj.deleted), partitionname(obj.partitionname), graph(obj.graph), graphstatus(obj.graphstatus), graphname(obj.graphname) {
}
const Job& Job::operator=(const Job& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        uuid = obj.uuid;
        created = obj.created;
        begintime = obj.begintime;
        endtime = obj.endtime;
        status = obj.status;
        infile = obj.infile;
        outfile = obj.outfile;
        starttime = obj.starttime;
        duration = obj.duration;
        progress = obj.progress;
        fps = obj.fps;
        data = obj.data;
        deleted = obj.deleted;
        partitionname = obj.partitionname;
        graph = obj.graph;
        graphstatus = obj.graphstatus;
        graphname = obj.graphname;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Job::TasksHandle Job::tasks() {
    return Job::TasksHandle(*this);
}
Job::JoblogHandle Job::joblog() {
    return Job::JoblogHandle(*this);
}
Job::InputfileHandle Job::inputfile() {
    return Job::InputfileHandle(*this);
}
Job::OutputfileHandle Job::outputfile() {
    return Job::OutputfileHandle(*this);
}
Job::PresetHandle Job::preset() {
    return Job::PresetHandle(*this);
}
Job::JobdetailsHandle Job::jobdetails() {
    return Job::JobdetailsHandle(*this);
}
Job::ProcessUnitsHandle Job::processUnits() {
    return Job::ProcessUnitsHandle(*this);
}
Job::PartitionHandle Job::partition() {
    return Job::PartitionHandle(*this);
}
Job::QueuesHandle Job::queues() {
    return Job::QueuesHandle(*this);
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
    fields.push_back(uuid.name());
    values.push_back(uuid);
    uuid.setModified(false);
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
    fields.push_back(fps.name());
    values.push_back(fps);
    fps.setModified(false);
    fields.push_back(data.name());
    values.push_back(data);
    data.setModified(false);
    fields.push_back(deleted.name());
    values.push_back(deleted);
    deleted.setModified(false);
    fields.push_back(partitionname.name());
    values.push_back(partitionname);
    partitionname.setModified(false);
    fields.push_back(graph.name());
    values.push_back(graph);
    graph.setModified(false);
    fields.push_back(graphstatus.name());
    values.push_back(graphstatus);
    graphstatus.setModified(false);
    fields.push_back(graphname.name());
    values.push_back(graphname);
    graphname.setModified(false);
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
    updateField(updates, table__, uuid);
    updateField(updates, table__, created);
    updateField(updates, table__, begintime);
    updateField(updates, table__, endtime);
    updateField(updates, table__, status);
    updateField(updates, table__, infile);
    updateField(updates, table__, outfile);
    updateField(updates, table__, starttime);
    updateField(updates, table__, duration);
    updateField(updates, table__, progress);
    updateField(updates, table__, fps);
    updateField(updates, table__, data);
    updateField(updates, table__, deleted);
    updateField(updates, table__, partitionname);
    updateField(updates, table__, graph);
    updateField(updates, table__, graphstatus);
    updateField(updates, table__, graphname);
}
void Job::addIDUpdates(Updates& updates) {
}
void Job::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Uuid);
    ftypes.push_back(Created);
    ftypes.push_back(Begintime);
    ftypes.push_back(Endtime);
    ftypes.push_back(Status);
    ftypes.push_back(Infile);
    ftypes.push_back(Outfile);
    ftypes.push_back(Starttime);
    ftypes.push_back(Duration);
    ftypes.push_back(Progress);
    ftypes.push_back(Fps);
    ftypes.push_back(Data);
    ftypes.push_back(Deleted);
    ftypes.push_back(Partitionname);
    ftypes.push_back(Graph);
    ftypes.push_back(Graphstatus);
    ftypes.push_back(Graphname);
}
void Job::delRecord() {
    deleteFromTable(table__, id);
}
void Job::delRelations() {
    JobTaskRelationJobTask::del(*db, (JobTaskRelationJobTask::Job == id));
    JobJobLogRelationJobJobLog::del(*db, (JobJobLogRelationJobJobLog::Job == id));
    JobMediaFileRelationJobInFile::del(*db, (JobMediaFileRelationJobInFile::Job == id));
    JobMediaFileRelationJobOutFile::del(*db, (JobMediaFileRelationJobOutFile::Job == id));
    JobPresetRelation::del(*db, (JobPresetRelation::Job == id));
    JobJobDetailRelationJobJobDetail::del(*db, (JobJobDetailRelationJobJobDetail::Job == id));
    JobProcessUnitRelationProcessUnitJob::del(*db, (JobProcessUnitRelationProcessUnitJob::Job == id));
    JobPartitionRelationJob2Partition::del(*db, (JobPartitionRelationJob2Partition::Job == id));
    JobQueueRelationJob2Queue::del(*db, (JobQueueRelationJob2Queue::Job == id));
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
    if (!typeIsCorrect()) {
        std::auto_ptr<Job> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Job::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<Job> Job::upcast() const {
    return auto_ptr<Job>(new Job(*this));
}
std::auto_ptr<Job> Job::upcastCopy() const {
    Job* np = new Job(*this);
    np->id = id;
    np->type = type;
    np->uuid = uuid;
    np->created = created;
    np->begintime = begintime;
    np->endtime = endtime;
    np->status = status;
    np->infile = infile;
    np->outfile = outfile;
    np->starttime = starttime;
    np->duration = duration;
    np->progress = progress;
    np->fps = fps;
    np->data = data;
    np->deleted = deleted;
    np->partitionname = partitionname;
    np->graph = graph;
    np->graphstatus = graphstatus;
    np->graphname = graphname;
    np->inDatabase = inDatabase;
    return auto_ptr<Job>(np);
}
std::ostream & operator<<(std::ostream& os, Job o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.uuid.name() << " = " << o.uuid << std::endl;
    os << o.created.name() << " = " << o.created << std::endl;
    os << o.begintime.name() << " = " << o.begintime << std::endl;
    os << o.endtime.name() << " = " << o.endtime << std::endl;
    os << o.status.name() << " = " << o.status << std::endl;
    os << o.infile.name() << " = " << o.infile << std::endl;
    os << o.outfile.name() << " = " << o.outfile << std::endl;
    os << o.starttime.name() << " = " << o.starttime << std::endl;
    os << o.duration.name() << " = " << o.duration << std::endl;
    os << o.progress.name() << " = " << o.progress << std::endl;
    os << o.fps.name() << " = " << o.fps << std::endl;
    os << o.data.name() << " = " << o.data << std::endl;
    os << o.deleted.name() << " = " << o.deleted << std::endl;
    os << o.partitionname.name() << " = " << o.partitionname << std::endl;
    os << o.graph.name() << " = " << o.graph << std::endl;
    os << o.graphstatus.name() << " = " << o.graphstatus << std::endl;
    os << o.graphname.name() << " = " << o.graphname << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Task::Own::Id("id_",A_field_type_integer,"Task_");
const int Task::StatusType::Waiting(0);
const int Task::StatusType::Error(1);
const int Task::StatusType::Complete(2);
const int Task::StatusType::Processing(3);
Task::StatusType::StatusType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals)
         : litesql::FieldType(n,t,tbl,vals) {
}
const int Task::Status::Waiting(0);
const int Task::Status::Error(1);
const int Task::Status::Complete(2);
const int Task::Status::Processing(3);
Task::JobHandle::JobHandle(const Task& owner)
         : litesql::RelationHandle<Task>(owner) {
}
void Task::JobHandle::link(const Job& o0) {
    JobTaskRelationJobTask::link(owner->getDatabase(), o0, *owner);
}
void Task::JobHandle::unlink(const Job& o0) {
    JobTaskRelationJobTask::unlink(owner->getDatabase(), o0, *owner);
}
void Task::JobHandle::del(const litesql::Expr& expr) {
    JobTaskRelationJobTask::del(owner->getDatabase(), expr && JobTaskRelationJobTask::Task == owner->id);
}
litesql::DataSource<Job> Task::JobHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobTaskRelationJobTask::get<Job>(owner->getDatabase(), expr, (JobTaskRelationJobTask::Task == owner->id) && srcExpr);
}
litesql::DataSource<JobTaskRelationJobTask::Row> Task::JobHandle::getRows(const litesql::Expr& expr) {
    return JobTaskRelationJobTask::getRows(owner->getDatabase(), expr && (JobTaskRelationJobTask::Task == owner->id));
}
Task::JobTaskHandle::JobTaskHandle(const Task& owner)
         : litesql::RelationHandle<Task>(owner) {
}
void Task::JobTaskHandle::link(const Graph& o0) {
    GraphTaskRelationGraphTask::link(owner->getDatabase(), o0, *owner);
}
void Task::JobTaskHandle::unlink(const Graph& o0) {
    GraphTaskRelationGraphTask::unlink(owner->getDatabase(), o0, *owner);
}
void Task::JobTaskHandle::del(const litesql::Expr& expr) {
    GraphTaskRelationGraphTask::del(owner->getDatabase(), expr && GraphTaskRelationGraphTask::Task == owner->id);
}
litesql::DataSource<Graph> Task::JobTaskHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return GraphTaskRelationGraphTask::get<Graph>(owner->getDatabase(), expr, (GraphTaskRelationGraphTask::Task == owner->id) && srcExpr);
}
litesql::DataSource<GraphTaskRelationGraphTask::Row> Task::JobTaskHandle::getRows(const litesql::Expr& expr) {
    return GraphTaskRelationGraphTask::getRows(owner->getDatabase(), expr && (GraphTaskRelationGraphTask::Task == owner->id));
}
const std::string Task::type__("Task");
const std::string Task::table__("Task_");
const std::string Task::sequence__("Task_seq");
const litesql::FieldType Task::Id("id_",A_field_type_integer,table__);
const litesql::FieldType Task::Type("type_",A_field_type_string,table__);
const litesql::FieldType Task::Uuid("uuid_",A_field_type_string,table__);
const litesql::FieldType Task::Name("name_",A_field_type_string,table__);
const litesql::FieldType Task::Parameter("parameter_",A_field_type_string,table__);
const litesql::FieldType Task::Statustext("statustext_",A_field_type_string,table__);
const litesql::FieldType Task::Progress("progress_",A_field_type_integer,table__);
std::vector < std::pair< std::string, std::string > > Task::status_values;
const Task::StatusType Task::Status("status_",A_field_type_integer,table__,status_values);
const Task::StatusType Task::Sortorder("sortorder_",A_field_type_integer,table__);
void Task::initValues() {
    status_values.clear();
    status_values.push_back(make_pair<std::string, std::string>("Waiting","0"));
    status_values.push_back(make_pair<std::string, std::string>("Error","1"));
    status_values.push_back(make_pair<std::string, std::string>("Complete","2"));
    status_values.push_back(make_pair<std::string, std::string>("Processing","3"));
}
void Task::defaults() {
    id = 0;
    progress = 0;
    status = 0;
    sortorder = 0;
}
Task::Task(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), uuid(Uuid), name(Name), parameter(Parameter), statustext(Statustext), progress(Progress), status(Status), sortorder(Sortorder) {
    defaults();
}
Task::Task(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), uuid(Uuid), name(Name), parameter(Parameter), statustext(Statustext), progress(Progress), status(Status), sortorder(Sortorder) {
    defaults();
    size_t size = (rec.size() > 9) ? 9 : rec.size();
    switch(size) {
    case 9: sortorder = convert<const std::string&, int>(rec[8]);
        sortorder.setModified(false);
    case 8: status = convert<const std::string&, int>(rec[7]);
        status.setModified(false);
    case 7: progress = convert<const std::string&, int>(rec[6]);
        progress.setModified(false);
    case 6: statustext = convert<const std::string&, std::string>(rec[5]);
        statustext.setModified(false);
    case 5: parameter = convert<const std::string&, std::string>(rec[4]);
        parameter.setModified(false);
    case 4: name = convert<const std::string&, std::string>(rec[3]);
        name.setModified(false);
    case 3: uuid = convert<const std::string&, std::string>(rec[2]);
        uuid.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Task::Task(const Task& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), uuid(obj.uuid), name(obj.name), parameter(obj.parameter), statustext(obj.statustext), progress(obj.progress), status(obj.status), sortorder(obj.sortorder) {
}
const Task& Task::operator=(const Task& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        uuid = obj.uuid;
        name = obj.name;
        parameter = obj.parameter;
        statustext = obj.statustext;
        progress = obj.progress;
        status = obj.status;
        sortorder = obj.sortorder;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Task::JobHandle Task::job() {
    return Task::JobHandle(*this);
}
Task::JobTaskHandle Task::jobTask() {
    return Task::JobTaskHandle(*this);
}
std::string Task::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(uuid.name());
    values.push_back(uuid);
    uuid.setModified(false);
    fields.push_back(name.name());
    values.push_back(name);
    name.setModified(false);
    fields.push_back(parameter.name());
    values.push_back(parameter);
    parameter.setModified(false);
    fields.push_back(statustext.name());
    values.push_back(statustext);
    statustext.setModified(false);
    fields.push_back(progress.name());
    values.push_back(progress);
    progress.setModified(false);
    fields.push_back(status.name());
    values.push_back(status);
    status.setModified(false);
    fields.push_back(sortorder.name());
    values.push_back(sortorder);
    sortorder.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Task::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Task::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, uuid);
    updateField(updates, table__, name);
    updateField(updates, table__, parameter);
    updateField(updates, table__, statustext);
    updateField(updates, table__, progress);
    updateField(updates, table__, status);
    updateField(updates, table__, sortorder);
}
void Task::addIDUpdates(Updates& updates) {
}
void Task::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Uuid);
    ftypes.push_back(Name);
    ftypes.push_back(Parameter);
    ftypes.push_back(Statustext);
    ftypes.push_back(Progress);
    ftypes.push_back(Status);
    ftypes.push_back(Sortorder);
}
void Task::delRecord() {
    deleteFromTable(table__, id);
}
void Task::delRelations() {
    JobTaskRelationJobTask::del(*db, (JobTaskRelationJobTask::Task == id));
    GraphTaskRelationGraphTask::del(*db, (GraphTaskRelationGraphTask::Task == id));
}
void Task::update() {
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
void Task::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<Task> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Task::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<Task> Task::upcast() const {
    return auto_ptr<Task>(new Task(*this));
}
std::auto_ptr<Task> Task::upcastCopy() const {
    Task* np = new Task(*this);
    np->id = id;
    np->type = type;
    np->uuid = uuid;
    np->name = name;
    np->parameter = parameter;
    np->statustext = statustext;
    np->progress = progress;
    np->status = status;
    np->sortorder = sortorder;
    np->inDatabase = inDatabase;
    return auto_ptr<Task>(np);
}
std::ostream & operator<<(std::ostream& os, Task o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.uuid.name() << " = " << o.uuid << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.parameter.name() << " = " << o.parameter << std::endl;
    os << o.statustext.name() << " = " << o.statustext << std::endl;
    os << o.progress.name() << " = " << o.progress << std::endl;
    os << o.status.name() << " = " << o.status << std::endl;
    os << o.sortorder.name() << " = " << o.sortorder << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType JobLog::Own::Id("id_",A_field_type_integer,"JobLog_");
JobLog::LogjobHandle::LogjobHandle(const JobLog& owner)
         : litesql::RelationHandle<JobLog>(owner) {
}
void JobLog::LogjobHandle::link(const Job& o0) {
    JobJobLogRelationJobJobLog::link(owner->getDatabase(), o0, *owner);
}
void JobLog::LogjobHandle::unlink(const Job& o0) {
    JobJobLogRelationJobJobLog::unlink(owner->getDatabase(), o0, *owner);
}
void JobLog::LogjobHandle::del(const litesql::Expr& expr) {
    JobJobLogRelationJobJobLog::del(owner->getDatabase(), expr && JobJobLogRelationJobJobLog::JobLog == owner->id);
}
litesql::DataSource<Job> JobLog::LogjobHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobJobLogRelationJobJobLog::get<Job>(owner->getDatabase(), expr, (JobJobLogRelationJobJobLog::JobLog == owner->id) && srcExpr);
}
litesql::DataSource<JobJobLogRelationJobJobLog::Row> JobLog::LogjobHandle::getRows(const litesql::Expr& expr) {
    return JobJobLogRelationJobJobLog::getRows(owner->getDatabase(), expr && (JobJobLogRelationJobJobLog::JobLog == owner->id));
}
const std::string JobLog::type__("JobLog");
const std::string JobLog::table__("JobLog_");
const std::string JobLog::sequence__("JobLog_seq");
const litesql::FieldType JobLog::Id("id_",A_field_type_integer,table__);
const litesql::FieldType JobLog::Type("type_",A_field_type_string,table__);
const litesql::FieldType JobLog::Created("created_",A_field_type_datetime,table__);
const litesql::FieldType JobLog::Message("message_",A_field_type_string,table__);
void JobLog::defaults() {
    id = 0;
    created = 0;
}
JobLog::JobLog(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), created(Created), message(Message) {
    defaults();
}
JobLog::JobLog(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), created(Created), message(Message) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: message = convert<const std::string&, std::string>(rec[3]);
        message.setModified(false);
    case 3: created = convert<const std::string&, litesql::DateTime>(rec[2]);
        created.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
JobLog::JobLog(const JobLog& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), created(obj.created), message(obj.message) {
}
const JobLog& JobLog::operator=(const JobLog& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        created = obj.created;
        message = obj.message;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
JobLog::LogjobHandle JobLog::logjob() {
    return JobLog::LogjobHandle(*this);
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
    updateField(updates, table__, message);
}
void JobLog::addIDUpdates(Updates& updates) {
}
void JobLog::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Created);
    ftypes.push_back(Message);
}
void JobLog::delRecord() {
    deleteFromTable(table__, id);
}
void JobLog::delRelations() {
    JobJobLogRelationJobJobLog::del(*db, (JobJobLogRelationJobJobLog::JobLog == id));
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
    if (!typeIsCorrect()) {
        std::auto_ptr<JobLog> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool JobLog::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<JobLog> JobLog::upcast() const {
    return auto_ptr<JobLog>(new JobLog(*this));
}
std::auto_ptr<JobLog> JobLog::upcastCopy() const {
    JobLog* np = new JobLog(*this);
    np->id = id;
    np->type = type;
    np->created = created;
    np->message = message;
    np->inDatabase = inDatabase;
    return auto_ptr<JobLog>(np);
}
std::ostream & operator<<(std::ostream& os, JobLog o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.created.name() << " = " << o.created << std::endl;
    os << o.message.name() << " = " << o.message << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType JobDetail::Own::Id("id_",A_field_type_integer,"JobDetail_");
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
const litesql::FieldType JobDetail::Id("id_",A_field_type_integer,table__);
const litesql::FieldType JobDetail::Type("type_",A_field_type_string,table__);
const litesql::FieldType JobDetail::Lastpts("lastpts_",A_field_type_double,table__);
const litesql::FieldType JobDetail::Lastdts("lastdts_",A_field_type_double,table__);
const litesql::FieldType JobDetail::Deinterlace("deinterlace_",A_field_type_integer,table__);
void JobDetail::defaults() {
    id = 0;
    lastpts = 0.0;
    lastdts = 0.0;
    deinterlace = 0;
}
JobDetail::JobDetail(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), lastpts(Lastpts), lastdts(Lastdts), deinterlace(Deinterlace) {
    defaults();
}
JobDetail::JobDetail(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), lastpts(Lastpts), lastdts(Lastdts), deinterlace(Deinterlace) {
    defaults();
    size_t size = (rec.size() > 5) ? 5 : rec.size();
    switch(size) {
    case 5: deinterlace = convert<const std::string&, int>(rec[4]);
        deinterlace.setModified(false);
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
     : litesql::Persistent(obj), id(obj.id), type(obj.type), lastpts(obj.lastpts), lastdts(obj.lastdts), deinterlace(obj.deinterlace) {
}
const JobDetail& JobDetail::operator=(const JobDetail& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        lastpts = obj.lastpts;
        lastdts = obj.lastdts;
        deinterlace = obj.deinterlace;
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
    fields.push_back(deinterlace.name());
    values.push_back(deinterlace);
    deinterlace.setModified(false);
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
    updateField(updates, table__, deinterlace);
}
void JobDetail::addIDUpdates(Updates& updates) {
}
void JobDetail::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Lastpts);
    ftypes.push_back(Lastdts);
    ftypes.push_back(Deinterlace);
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
    if (!typeIsCorrect()) {
        std::auto_ptr<JobDetail> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool JobDetail::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<JobDetail> JobDetail::upcast() const {
    return auto_ptr<JobDetail>(new JobDetail(*this));
}
std::auto_ptr<JobDetail> JobDetail::upcastCopy() const {
    JobDetail* np = new JobDetail(*this);
    np->id = id;
    np->type = type;
    np->lastpts = lastpts;
    np->lastdts = lastdts;
    np->deinterlace = deinterlace;
    np->inDatabase = inDatabase;
    return auto_ptr<JobDetail>(np);
}
std::ostream & operator<<(std::ostream& os, JobDetail o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.lastpts.name() << " = " << o.lastpts << std::endl;
    os << o.lastdts.name() << " = " << o.lastdts << std::endl;
    os << o.deinterlace.name() << " = " << o.deinterlace << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Watchfolder::Own::Id("id_",A_field_type_integer,"Watchfolder_");
const std::string Watchfolder::type__("Watchfolder");
const std::string Watchfolder::table__("Watchfolder_");
const std::string Watchfolder::sequence__("Watchfolder_seq");
const litesql::FieldType Watchfolder::Id("id_",A_field_type_integer,table__);
const litesql::FieldType Watchfolder::Type("type_",A_field_type_string,table__);
const litesql::FieldType Watchfolder::Folder("folder_",A_field_type_string,table__);
const litesql::FieldType Watchfolder::Filefilter("filefilter_",A_field_type_string,table__);
const litesql::FieldType Watchfolder::Subdirs("subdirs_",A_field_type_integer,table__);
const litesql::FieldType Watchfolder::Flowuuid("flowuuid_",A_field_type_string,table__);
const litesql::FieldType Watchfolder::Interval("interval_",A_field_type_integer,table__);
const litesql::FieldType Watchfolder::Start("start_",A_field_type_datetime,table__);
const litesql::FieldType Watchfolder::End("end_",A_field_type_datetime,table__);
void Watchfolder::defaults() {
    id = 0;
    subdirs = 0;
    interval = 0;
    start = 0;
    end = 0;
}
Watchfolder::Watchfolder(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), folder(Folder), filefilter(Filefilter), subdirs(Subdirs), flowuuid(Flowuuid), interval(Interval), start(Start), end(End) {
    defaults();
}
Watchfolder::Watchfolder(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), folder(Folder), filefilter(Filefilter), subdirs(Subdirs), flowuuid(Flowuuid), interval(Interval), start(Start), end(End) {
    defaults();
    size_t size = (rec.size() > 9) ? 9 : rec.size();
    switch(size) {
    case 9: end = convert<const std::string&, litesql::DateTime>(rec[8]);
        end.setModified(false);
    case 8: start = convert<const std::string&, litesql::DateTime>(rec[7]);
        start.setModified(false);
    case 7: interval = convert<const std::string&, int>(rec[6]);
        interval.setModified(false);
    case 6: flowuuid = convert<const std::string&, std::string>(rec[5]);
        flowuuid.setModified(false);
    case 5: subdirs = convert<const std::string&, int>(rec[4]);
        subdirs.setModified(false);
    case 4: filefilter = convert<const std::string&, std::string>(rec[3]);
        filefilter.setModified(false);
    case 3: folder = convert<const std::string&, std::string>(rec[2]);
        folder.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Watchfolder::Watchfolder(const Watchfolder& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), folder(obj.folder), filefilter(obj.filefilter), subdirs(obj.subdirs), flowuuid(obj.flowuuid), interval(obj.interval), start(obj.start), end(obj.end) {
}
const Watchfolder& Watchfolder::operator=(const Watchfolder& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        folder = obj.folder;
        filefilter = obj.filefilter;
        subdirs = obj.subdirs;
        flowuuid = obj.flowuuid;
        interval = obj.interval;
        start = obj.start;
        end = obj.end;
    }
    litesql::Persistent::operator=(obj);
    return *this;
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
    fields.push_back(folder.name());
    values.push_back(folder);
    folder.setModified(false);
    fields.push_back(filefilter.name());
    values.push_back(filefilter);
    filefilter.setModified(false);
    fields.push_back(subdirs.name());
    values.push_back(subdirs);
    subdirs.setModified(false);
    fields.push_back(flowuuid.name());
    values.push_back(flowuuid);
    flowuuid.setModified(false);
    fields.push_back(interval.name());
    values.push_back(interval);
    interval.setModified(false);
    fields.push_back(start.name());
    values.push_back(start);
    start.setModified(false);
    fields.push_back(end.name());
    values.push_back(end);
    end.setModified(false);
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
    updateField(updates, table__, folder);
    updateField(updates, table__, filefilter);
    updateField(updates, table__, subdirs);
    updateField(updates, table__, flowuuid);
    updateField(updates, table__, interval);
    updateField(updates, table__, start);
    updateField(updates, table__, end);
}
void Watchfolder::addIDUpdates(Updates& updates) {
}
void Watchfolder::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Folder);
    ftypes.push_back(Filefilter);
    ftypes.push_back(Subdirs);
    ftypes.push_back(Flowuuid);
    ftypes.push_back(Interval);
    ftypes.push_back(Start);
    ftypes.push_back(End);
}
void Watchfolder::delRecord() {
    deleteFromTable(table__, id);
}
void Watchfolder::delRelations() {
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
    if (!typeIsCorrect()) {
        std::auto_ptr<Watchfolder> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Watchfolder::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<Watchfolder> Watchfolder::upcast() const {
    return auto_ptr<Watchfolder>(new Watchfolder(*this));
}
std::auto_ptr<Watchfolder> Watchfolder::upcastCopy() const {
    Watchfolder* np = new Watchfolder(*this);
    np->id = id;
    np->type = type;
    np->folder = folder;
    np->filefilter = filefilter;
    np->subdirs = subdirs;
    np->flowuuid = flowuuid;
    np->interval = interval;
    np->start = start;
    np->end = end;
    np->inDatabase = inDatabase;
    return auto_ptr<Watchfolder>(np);
}
std::ostream & operator<<(std::ostream& os, Watchfolder o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.folder.name() << " = " << o.folder << std::endl;
    os << o.filefilter.name() << " = " << o.filefilter << std::endl;
    os << o.subdirs.name() << " = " << o.subdirs << std::endl;
    os << o.flowuuid.name() << " = " << o.flowuuid << std::endl;
    os << o.interval.name() << " = " << o.interval << std::endl;
    os << o.start.name() << " = " << o.start << std::endl;
    os << o.end.name() << " = " << o.end << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Process::Own::Id("id_",A_field_type_integer,"Process_");
const std::string Process::type__("Process");
const std::string Process::table__("Process_");
const std::string Process::sequence__("Process_seq");
const litesql::FieldType Process::Id("id_",A_field_type_integer,table__);
const litesql::FieldType Process::Type("type_",A_field_type_string,table__);
const litesql::FieldType Process::Executable("executable_",A_field_type_string,table__);
const litesql::FieldType Process::Arguments("arguments_",A_field_type_string,table__);
const litesql::FieldType Process::Name("name_",A_field_type_string,table__);
const litesql::FieldType Process::Pid("pid_",A_field_type_integer,table__);
const litesql::FieldType Process::Started("started_",A_field_type_datetime,table__);
const litesql::FieldType Process::Stopped("stopped_",A_field_type_datetime,table__);
void Process::defaults() {
    id = 0;
    pid = 0;
    started = 0;
    stopped = 0;
}
Process::Process(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), executable(Executable), arguments(Arguments), name(Name), pid(Pid), started(Started), stopped(Stopped) {
    defaults();
}
Process::Process(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), executable(Executable), arguments(Arguments), name(Name), pid(Pid), started(Started), stopped(Stopped) {
    defaults();
    size_t size = (rec.size() > 8) ? 8 : rec.size();
    switch(size) {
    case 8: stopped = convert<const std::string&, litesql::DateTime>(rec[7]);
        stopped.setModified(false);
    case 7: started = convert<const std::string&, litesql::DateTime>(rec[6]);
        started.setModified(false);
    case 6: pid = convert<const std::string&, int>(rec[5]);
        pid.setModified(false);
    case 5: name = convert<const std::string&, std::string>(rec[4]);
        name.setModified(false);
    case 4: arguments = convert<const std::string&, std::string>(rec[3]);
        arguments.setModified(false);
    case 3: executable = convert<const std::string&, std::string>(rec[2]);
        executable.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Process::Process(const Process& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), executable(obj.executable), arguments(obj.arguments), name(obj.name), pid(obj.pid), started(obj.started), stopped(obj.stopped) {
}
const Process& Process::operator=(const Process& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        executable = obj.executable;
        arguments = obj.arguments;
        name = obj.name;
        pid = obj.pid;
        started = obj.started;
        stopped = obj.stopped;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string Process::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(executable.name());
    values.push_back(executable);
    executable.setModified(false);
    fields.push_back(arguments.name());
    values.push_back(arguments);
    arguments.setModified(false);
    fields.push_back(name.name());
    values.push_back(name);
    name.setModified(false);
    fields.push_back(pid.name());
    values.push_back(pid);
    pid.setModified(false);
    fields.push_back(started.name());
    values.push_back(started);
    started.setModified(false);
    fields.push_back(stopped.name());
    values.push_back(stopped);
    stopped.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Process::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Process::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, executable);
    updateField(updates, table__, arguments);
    updateField(updates, table__, name);
    updateField(updates, table__, pid);
    updateField(updates, table__, started);
    updateField(updates, table__, stopped);
}
void Process::addIDUpdates(Updates& updates) {
}
void Process::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Executable);
    ftypes.push_back(Arguments);
    ftypes.push_back(Name);
    ftypes.push_back(Pid);
    ftypes.push_back(Started);
    ftypes.push_back(Stopped);
}
void Process::delRecord() {
    deleteFromTable(table__, id);
}
void Process::delRelations() {
}
void Process::update() {
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
void Process::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<Process> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Process::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<Process> Process::upcast() const {
    return auto_ptr<Process>(new Process(*this));
}
std::auto_ptr<Process> Process::upcastCopy() const {
    Process* np = new Process(*this);
    np->id = id;
    np->type = type;
    np->executable = executable;
    np->arguments = arguments;
    np->name = name;
    np->pid = pid;
    np->started = started;
    np->stopped = stopped;
    np->inDatabase = inDatabase;
    return auto_ptr<Process>(np);
}
std::ostream & operator<<(std::ostream& os, Process o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.executable.name() << " = " << o.executable << std::endl;
    os << o.arguments.name() << " = " << o.arguments << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.pid.name() << " = " << o.pid << std::endl;
    os << o.started.name() << " = " << o.started << std::endl;
    os << o.stopped.name() << " = " << o.stopped << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType ProcessUnit::Own::Id("id_",A_field_type_integer,"ProcessUnit_");
const std::string ProcessUnit::CodectypeType::AUDIO("AUDIO");
const std::string ProcessUnit::CodectypeType::VIDEO("VIDEO");
ProcessUnit::CodectypeType::CodectypeType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals)
         : litesql::FieldType(n,t,tbl,vals) {
}
const std::string ProcessUnit::Codectype::AUDIO("AUDIO");
const std::string ProcessUnit::Codectype::VIDEO("VIDEO");
ProcessUnit::JobHandle::JobHandle(const ProcessUnit& owner)
         : litesql::RelationHandle<ProcessUnit>(owner) {
}
void ProcessUnit::JobHandle::link(const Job& o0) {
    JobProcessUnitRelationProcessUnitJob::link(owner->getDatabase(), o0, *owner);
}
void ProcessUnit::JobHandle::unlink(const Job& o0) {
    JobProcessUnitRelationProcessUnitJob::unlink(owner->getDatabase(), o0, *owner);
}
void ProcessUnit::JobHandle::del(const litesql::Expr& expr) {
    JobProcessUnitRelationProcessUnitJob::del(owner->getDatabase(), expr && JobProcessUnitRelationProcessUnitJob::ProcessUnit == owner->id);
}
litesql::DataSource<Job> ProcessUnit::JobHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobProcessUnitRelationProcessUnitJob::get<Job>(owner->getDatabase(), expr, (JobProcessUnitRelationProcessUnitJob::ProcessUnit == owner->id) && srcExpr);
}
litesql::DataSource<JobProcessUnitRelationProcessUnitJob::Row> ProcessUnit::JobHandle::getRows(const litesql::Expr& expr) {
    return JobProcessUnitRelationProcessUnitJob::getRows(owner->getDatabase(), expr && (JobProcessUnitRelationProcessUnitJob::ProcessUnit == owner->id));
}
const std::string ProcessUnit::type__("ProcessUnit");
const std::string ProcessUnit::table__("ProcessUnit_");
const std::string ProcessUnit::sequence__("ProcessUnit_seq");
const litesql::FieldType ProcessUnit::Id("id_",A_field_type_integer,table__);
const litesql::FieldType ProcessUnit::Type("type_",A_field_type_string,table__);
const litesql::FieldType ProcessUnit::Sorcestream("sorcestream_",A_field_type_integer,table__);
const litesql::FieldType ProcessUnit::Targetstream("targetstream_",A_field_type_integer,table__);
const litesql::FieldType ProcessUnit::Timebasenum("timebasenum_",A_field_type_integer,table__);
const litesql::FieldType ProcessUnit::Timebaseden("timebaseden_",A_field_type_integer,table__);
const litesql::FieldType ProcessUnit::Startts("startts_",A_field_type_double,table__);
const litesql::FieldType ProcessUnit::Endts("endts_",A_field_type_double,table__);
const litesql::FieldType ProcessUnit::Framecount("framecount_",A_field_type_integer,table__);
const litesql::FieldType ProcessUnit::Send("send_",A_field_type_datetime,table__);
const litesql::FieldType ProcessUnit::Recv("recv_",A_field_type_datetime,table__);
const litesql::FieldType ProcessUnit::Sendid("sendid_",A_field_type_string,table__);
const litesql::FieldType ProcessUnit::Recvid("recvid_",A_field_type_string,table__);
const litesql::FieldType ProcessUnit::Data("data_",A_field_type_blob,table__);
const litesql::FieldType ProcessUnit::ResponseData("responseData_",A_field_type_blob,table__);
const litesql::FieldType ProcessUnit::Clientid("clientid_",A_field_type_string,table__);
const litesql::FieldType ProcessUnit::Deliverycount("deliverycount_",A_field_type_integer,table__);
const litesql::FieldType ProcessUnit::Jobid("jobid_",A_field_type_string,table__);
const litesql::FieldType ProcessUnit::Group("group_",A_field_type_string,table__);
const litesql::FieldType ProcessUnit::Sequence("sequence_",A_field_type_integer,table__);
std::vector < std::pair< std::string, std::string > > ProcessUnit::codectype_values;
const ProcessUnit::CodectypeType ProcessUnit::Codectype("codectype_",A_field_type_string,table__,codectype_values);
void ProcessUnit::initValues() {
    codectype_values.clear();
    codectype_values.push_back(make_pair<std::string, std::string>("AUDIO","AUDIO"));
    codectype_values.push_back(make_pair<std::string, std::string>("VIDEO","VIDEO"));
}
void ProcessUnit::defaults() {
    id = 0;
    sorcestream = 0;
    targetstream = 0;
    timebasenum = 0;
    timebaseden = 0;
    startts = 0.0;
    endts = 0.0;
    framecount = 0;
    send = 0;
    recv = 0;
    data = Blob::nil;
    responseData = Blob::nil;
    deliverycount = 0;
    sequence = 0;
    codectype = "0";
}
ProcessUnit::ProcessUnit(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), sorcestream(Sorcestream), targetstream(Targetstream), timebasenum(Timebasenum), timebaseden(Timebaseden), startts(Startts), endts(Endts), framecount(Framecount), send(Send), recv(Recv), sendid(Sendid), recvid(Recvid), data(Data), responseData(ResponseData), clientid(Clientid), deliverycount(Deliverycount), jobid(Jobid), group(Group), sequence(Sequence), codectype(Codectype) {
    defaults();
}
ProcessUnit::ProcessUnit(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), sorcestream(Sorcestream), targetstream(Targetstream), timebasenum(Timebasenum), timebaseden(Timebaseden), startts(Startts), endts(Endts), framecount(Framecount), send(Send), recv(Recv), sendid(Sendid), recvid(Recvid), data(Data), responseData(ResponseData), clientid(Clientid), deliverycount(Deliverycount), jobid(Jobid), group(Group), sequence(Sequence), codectype(Codectype) {
    defaults();
    size_t size = (rec.size() > 21) ? 21 : rec.size();
    switch(size) {
    case 21: codectype = convert<const std::string&, std::string>(rec[20]);
        codectype.setModified(false);
    case 20: sequence = convert<const std::string&, int>(rec[19]);
        sequence.setModified(false);
    case 19: group = convert<const std::string&, std::string>(rec[18]);
        group.setModified(false);
    case 18: jobid = convert<const std::string&, std::string>(rec[17]);
        jobid.setModified(false);
    case 17: deliverycount = convert<const std::string&, int>(rec[16]);
        deliverycount.setModified(false);
    case 16: clientid = convert<const std::string&, std::string>(rec[15]);
        clientid.setModified(false);
    case 15: responseData = convert<const std::string&, litesql::Blob>(rec[14]);
        responseData.setModified(false);
    case 14: data = convert<const std::string&, litesql::Blob>(rec[13]);
        data.setModified(false);
    case 13: recvid = convert<const std::string&, std::string>(rec[12]);
        recvid.setModified(false);
    case 12: sendid = convert<const std::string&, std::string>(rec[11]);
        sendid.setModified(false);
    case 11: recv = convert<const std::string&, litesql::DateTime>(rec[10]);
        recv.setModified(false);
    case 10: send = convert<const std::string&, litesql::DateTime>(rec[9]);
        send.setModified(false);
    case 9: framecount = convert<const std::string&, int>(rec[8]);
        framecount.setModified(false);
    case 8: endts = convert<const std::string&, double>(rec[7]);
        endts.setModified(false);
    case 7: startts = convert<const std::string&, double>(rec[6]);
        startts.setModified(false);
    case 6: timebaseden = convert<const std::string&, int>(rec[5]);
        timebaseden.setModified(false);
    case 5: timebasenum = convert<const std::string&, int>(rec[4]);
        timebasenum.setModified(false);
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
     : litesql::Persistent(obj), id(obj.id), type(obj.type), sorcestream(obj.sorcestream), targetstream(obj.targetstream), timebasenum(obj.timebasenum), timebaseden(obj.timebaseden), startts(obj.startts), endts(obj.endts), framecount(obj.framecount), send(obj.send), recv(obj.recv), sendid(obj.sendid), recvid(obj.recvid), data(obj.data), responseData(obj.responseData), clientid(obj.clientid), deliverycount(obj.deliverycount), jobid(obj.jobid), group(obj.group), sequence(obj.sequence), codectype(obj.codectype) {
}
const ProcessUnit& ProcessUnit::operator=(const ProcessUnit& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        sorcestream = obj.sorcestream;
        targetstream = obj.targetstream;
        timebasenum = obj.timebasenum;
        timebaseden = obj.timebaseden;
        startts = obj.startts;
        endts = obj.endts;
        framecount = obj.framecount;
        send = obj.send;
        recv = obj.recv;
        sendid = obj.sendid;
        recvid = obj.recvid;
        data = obj.data;
        responseData = obj.responseData;
        clientid = obj.clientid;
        deliverycount = obj.deliverycount;
        jobid = obj.jobid;
        group = obj.group;
        sequence = obj.sequence;
        codectype = obj.codectype;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
ProcessUnit::JobHandle ProcessUnit::job() {
    return ProcessUnit::JobHandle(*this);
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
    fields.push_back(timebasenum.name());
    values.push_back(timebasenum);
    timebasenum.setModified(false);
    fields.push_back(timebaseden.name());
    values.push_back(timebaseden);
    timebaseden.setModified(false);
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
    fields.push_back(sendid.name());
    values.push_back(sendid);
    sendid.setModified(false);
    fields.push_back(recvid.name());
    values.push_back(recvid);
    recvid.setModified(false);
    fields.push_back(data.name());
    values.push_back(data);
    data.setModified(false);
    fields.push_back(responseData.name());
    values.push_back(responseData);
    responseData.setModified(false);
    fields.push_back(clientid.name());
    values.push_back(clientid);
    clientid.setModified(false);
    fields.push_back(deliverycount.name());
    values.push_back(deliverycount);
    deliverycount.setModified(false);
    fields.push_back(jobid.name());
    values.push_back(jobid);
    jobid.setModified(false);
    fields.push_back(group.name());
    values.push_back(group);
    group.setModified(false);
    fields.push_back(sequence.name());
    values.push_back(sequence);
    sequence.setModified(false);
    fields.push_back(codectype.name());
    values.push_back(codectype);
    codectype.setModified(false);
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
    updateField(updates, table__, timebasenum);
    updateField(updates, table__, timebaseden);
    updateField(updates, table__, startts);
    updateField(updates, table__, endts);
    updateField(updates, table__, framecount);
    updateField(updates, table__, send);
    updateField(updates, table__, recv);
    updateField(updates, table__, sendid);
    updateField(updates, table__, recvid);
    updateField(updates, table__, data);
    updateField(updates, table__, responseData);
    updateField(updates, table__, clientid);
    updateField(updates, table__, deliverycount);
    updateField(updates, table__, jobid);
    updateField(updates, table__, group);
    updateField(updates, table__, sequence);
    updateField(updates, table__, codectype);
}
void ProcessUnit::addIDUpdates(Updates& updates) {
}
void ProcessUnit::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Sorcestream);
    ftypes.push_back(Targetstream);
    ftypes.push_back(Timebasenum);
    ftypes.push_back(Timebaseden);
    ftypes.push_back(Startts);
    ftypes.push_back(Endts);
    ftypes.push_back(Framecount);
    ftypes.push_back(Send);
    ftypes.push_back(Recv);
    ftypes.push_back(Sendid);
    ftypes.push_back(Recvid);
    ftypes.push_back(Data);
    ftypes.push_back(ResponseData);
    ftypes.push_back(Clientid);
    ftypes.push_back(Deliverycount);
    ftypes.push_back(Jobid);
    ftypes.push_back(Group);
    ftypes.push_back(Sequence);
    ftypes.push_back(Codectype);
}
void ProcessUnit::delRecord() {
    deleteFromTable(table__, id);
}
void ProcessUnit::delRelations() {
    JobProcessUnitRelationProcessUnitJob::del(*db, (JobProcessUnitRelationProcessUnitJob::ProcessUnit == id));
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
    if (!typeIsCorrect()) {
        std::auto_ptr<ProcessUnit> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool ProcessUnit::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<ProcessUnit> ProcessUnit::upcast() const {
    return auto_ptr<ProcessUnit>(new ProcessUnit(*this));
}
std::auto_ptr<ProcessUnit> ProcessUnit::upcastCopy() const {
    ProcessUnit* np = new ProcessUnit(*this);
    np->id = id;
    np->type = type;
    np->sorcestream = sorcestream;
    np->targetstream = targetstream;
    np->timebasenum = timebasenum;
    np->timebaseden = timebaseden;
    np->startts = startts;
    np->endts = endts;
    np->framecount = framecount;
    np->send = send;
    np->recv = recv;
    np->sendid = sendid;
    np->recvid = recvid;
    np->data = data;
    np->responseData = responseData;
    np->clientid = clientid;
    np->deliverycount = deliverycount;
    np->jobid = jobid;
    np->group = group;
    np->sequence = sequence;
    np->codectype = codectype;
    np->inDatabase = inDatabase;
    return auto_ptr<ProcessUnit>(np);
}
std::ostream & operator<<(std::ostream& os, ProcessUnit o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.sorcestream.name() << " = " << o.sorcestream << std::endl;
    os << o.targetstream.name() << " = " << o.targetstream << std::endl;
    os << o.timebasenum.name() << " = " << o.timebasenum << std::endl;
    os << o.timebaseden.name() << " = " << o.timebaseden << std::endl;
    os << o.startts.name() << " = " << o.startts << std::endl;
    os << o.endts.name() << " = " << o.endts << std::endl;
    os << o.framecount.name() << " = " << o.framecount << std::endl;
    os << o.send.name() << " = " << o.send << std::endl;
    os << o.recv.name() << " = " << o.recv << std::endl;
    os << o.sendid.name() << " = " << o.sendid << std::endl;
    os << o.recvid.name() << " = " << o.recvid << std::endl;
    os << o.data.name() << " = " << o.data << std::endl;
    os << o.responseData.name() << " = " << o.responseData << std::endl;
    os << o.clientid.name() << " = " << o.clientid << std::endl;
    os << o.deliverycount.name() << " = " << o.deliverycount << std::endl;
    os << o.jobid.name() << " = " << o.jobid << std::endl;
    os << o.group.name() << " = " << o.group << std::endl;
    os << o.sequence.name() << " = " << o.sequence << std::endl;
    os << o.codectype.name() << " = " << o.codectype << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType User::Own::Id("id_",A_field_type_integer,"User_");
User::PresetsHandle::PresetsHandle(const User& owner)
         : litesql::RelationHandle<User>(owner) {
}
void User::PresetsHandle::link(const Preset& o0) {
    PresetUserRelation::link(owner->getDatabase(), o0, *owner);
}
void User::PresetsHandle::unlink(const Preset& o0) {
    PresetUserRelation::unlink(owner->getDatabase(), o0, *owner);
}
void User::PresetsHandle::del(const litesql::Expr& expr) {
    PresetUserRelation::del(owner->getDatabase(), expr && PresetUserRelation::User == owner->id);
}
litesql::DataSource<Preset> User::PresetsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return PresetUserRelation::get<Preset>(owner->getDatabase(), expr, (PresetUserRelation::User == owner->id) && srcExpr);
}
litesql::DataSource<PresetUserRelation::Row> User::PresetsHandle::getRows(const litesql::Expr& expr) {
    return PresetUserRelation::getRows(owner->getDatabase(), expr && (PresetUserRelation::User == owner->id));
}
User::UserGroupHandle::UserGroupHandle(const User& owner)
         : litesql::RelationHandle<User>(owner) {
}
void User::UserGroupHandle::link(const UserGroup& o0) {
    UserUserGroupRelationUser2UserGroup::link(owner->getDatabase(), *owner, o0);
}
void User::UserGroupHandle::unlink(const UserGroup& o0) {
    UserUserGroupRelationUser2UserGroup::unlink(owner->getDatabase(), *owner, o0);
}
void User::UserGroupHandle::del(const litesql::Expr& expr) {
    UserUserGroupRelationUser2UserGroup::del(owner->getDatabase(), expr && UserUserGroupRelationUser2UserGroup::User == owner->id);
}
litesql::DataSource<UserGroup> User::UserGroupHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return UserUserGroupRelationUser2UserGroup::get<UserGroup>(owner->getDatabase(), expr, (UserUserGroupRelationUser2UserGroup::User == owner->id) && srcExpr);
}
litesql::DataSource<UserUserGroupRelationUser2UserGroup::Row> User::UserGroupHandle::getRows(const litesql::Expr& expr) {
    return UserUserGroupRelationUser2UserGroup::getRows(owner->getDatabase(), expr && (UserUserGroupRelationUser2UserGroup::User == owner->id));
}
const std::string User::type__("User");
const std::string User::table__("User_");
const std::string User::sequence__("User_seq");
const litesql::FieldType User::Id("id_",A_field_type_integer,table__);
const litesql::FieldType User::Type("type_",A_field_type_string,table__);
const litesql::FieldType User::Authname("authname_",A_field_type_string,table__);
const litesql::FieldType User::Authpass("authpass_",A_field_type_string,table__);
const litesql::FieldType User::Company("company_",A_field_type_string,table__);
const litesql::FieldType User::Firstname("firstname_",A_field_type_string,table__);
const litesql::FieldType User::Lastname("lastname_",A_field_type_string,table__);
const litesql::FieldType User::Street("street_",A_field_type_string,table__);
const litesql::FieldType User::City("city_",A_field_type_string,table__);
const litesql::FieldType User::Zip("zip_",A_field_type_string,table__);
const litesql::FieldType User::Country("country_",A_field_type_string,table__);
const litesql::FieldType User::State("state_",A_field_type_string,table__);
const litesql::FieldType User::Telefone("telefone_",A_field_type_string,table__);
const litesql::FieldType User::Fax("fax_",A_field_type_string,table__);
const litesql::FieldType User::Email("email_",A_field_type_string,table__);
const litesql::FieldType User::Www("www_",A_field_type_string,table__);
const litesql::FieldType User::Language("language_",A_field_type_string,table__);
const litesql::FieldType User::Licensekey("licensekey_",A_field_type_string,table__);
const litesql::FieldType User::Apikey("apikey_",A_field_type_string,table__);
const litesql::FieldType User::Registered("registered_",A_field_type_date,table__);
const litesql::FieldType User::Fileroot("fileroot_",A_field_type_string,table__);
void User::defaults() {
    id = 0;
    registered = 0;
}
User::User(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), authname(Authname), authpass(Authpass), company(Company), firstname(Firstname), lastname(Lastname), street(Street), city(City), zip(Zip), country(Country), state(State), telefone(Telefone), fax(Fax), email(Email), www(Www), language(Language), licensekey(Licensekey), apikey(Apikey), registered(Registered), fileroot(Fileroot) {
    defaults();
}
User::User(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), authname(Authname), authpass(Authpass), company(Company), firstname(Firstname), lastname(Lastname), street(Street), city(City), zip(Zip), country(Country), state(State), telefone(Telefone), fax(Fax), email(Email), www(Www), language(Language), licensekey(Licensekey), apikey(Apikey), registered(Registered), fileroot(Fileroot) {
    defaults();
    size_t size = (rec.size() > 21) ? 21 : rec.size();
    switch(size) {
    case 21: fileroot = convert<const std::string&, std::string>(rec[20]);
        fileroot.setModified(false);
    case 20: registered = convert<const std::string&, litesql::Date>(rec[19]);
        registered.setModified(false);
    case 19: apikey = convert<const std::string&, std::string>(rec[18]);
        apikey.setModified(false);
    case 18: licensekey = convert<const std::string&, std::string>(rec[17]);
        licensekey.setModified(false);
    case 17: language = convert<const std::string&, std::string>(rec[16]);
        language.setModified(false);
    case 16: www = convert<const std::string&, std::string>(rec[15]);
        www.setModified(false);
    case 15: email = convert<const std::string&, std::string>(rec[14]);
        email.setModified(false);
    case 14: fax = convert<const std::string&, std::string>(rec[13]);
        fax.setModified(false);
    case 13: telefone = convert<const std::string&, std::string>(rec[12]);
        telefone.setModified(false);
    case 12: state = convert<const std::string&, std::string>(rec[11]);
        state.setModified(false);
    case 11: country = convert<const std::string&, std::string>(rec[10]);
        country.setModified(false);
    case 10: zip = convert<const std::string&, std::string>(rec[9]);
        zip.setModified(false);
    case 9: city = convert<const std::string&, std::string>(rec[8]);
        city.setModified(false);
    case 8: street = convert<const std::string&, std::string>(rec[7]);
        street.setModified(false);
    case 7: lastname = convert<const std::string&, std::string>(rec[6]);
        lastname.setModified(false);
    case 6: firstname = convert<const std::string&, std::string>(rec[5]);
        firstname.setModified(false);
    case 5: company = convert<const std::string&, std::string>(rec[4]);
        company.setModified(false);
    case 4: authpass = convert<const std::string&, std::string>(rec[3]);
        authpass.setModified(false);
    case 3: authname = convert<const std::string&, std::string>(rec[2]);
        authname.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
User::User(const User& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), authname(obj.authname), authpass(obj.authpass), company(obj.company), firstname(obj.firstname), lastname(obj.lastname), street(obj.street), city(obj.city), zip(obj.zip), country(obj.country), state(obj.state), telefone(obj.telefone), fax(obj.fax), email(obj.email), www(obj.www), language(obj.language), licensekey(obj.licensekey), apikey(obj.apikey), registered(obj.registered), fileroot(obj.fileroot) {
}
const User& User::operator=(const User& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        authname = obj.authname;
        authpass = obj.authpass;
        company = obj.company;
        firstname = obj.firstname;
        lastname = obj.lastname;
        street = obj.street;
        city = obj.city;
        zip = obj.zip;
        country = obj.country;
        state = obj.state;
        telefone = obj.telefone;
        fax = obj.fax;
        email = obj.email;
        www = obj.www;
        language = obj.language;
        licensekey = obj.licensekey;
        apikey = obj.apikey;
        registered = obj.registered;
        fileroot = obj.fileroot;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
User::PresetsHandle User::presets() {
    return User::PresetsHandle(*this);
}
User::UserGroupHandle User::userGroup() {
    return User::UserGroupHandle(*this);
}
std::string User::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(authname.name());
    values.push_back(authname);
    authname.setModified(false);
    fields.push_back(authpass.name());
    values.push_back(authpass);
    authpass.setModified(false);
    fields.push_back(company.name());
    values.push_back(company);
    company.setModified(false);
    fields.push_back(firstname.name());
    values.push_back(firstname);
    firstname.setModified(false);
    fields.push_back(lastname.name());
    values.push_back(lastname);
    lastname.setModified(false);
    fields.push_back(street.name());
    values.push_back(street);
    street.setModified(false);
    fields.push_back(city.name());
    values.push_back(city);
    city.setModified(false);
    fields.push_back(zip.name());
    values.push_back(zip);
    zip.setModified(false);
    fields.push_back(country.name());
    values.push_back(country);
    country.setModified(false);
    fields.push_back(state.name());
    values.push_back(state);
    state.setModified(false);
    fields.push_back(telefone.name());
    values.push_back(telefone);
    telefone.setModified(false);
    fields.push_back(fax.name());
    values.push_back(fax);
    fax.setModified(false);
    fields.push_back(email.name());
    values.push_back(email);
    email.setModified(false);
    fields.push_back(www.name());
    values.push_back(www);
    www.setModified(false);
    fields.push_back(language.name());
    values.push_back(language);
    language.setModified(false);
    fields.push_back(licensekey.name());
    values.push_back(licensekey);
    licensekey.setModified(false);
    fields.push_back(apikey.name());
    values.push_back(apikey);
    apikey.setModified(false);
    fields.push_back(registered.name());
    values.push_back(registered);
    registered.setModified(false);
    fields.push_back(fileroot.name());
    values.push_back(fileroot);
    fileroot.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void User::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void User::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, authname);
    updateField(updates, table__, authpass);
    updateField(updates, table__, company);
    updateField(updates, table__, firstname);
    updateField(updates, table__, lastname);
    updateField(updates, table__, street);
    updateField(updates, table__, city);
    updateField(updates, table__, zip);
    updateField(updates, table__, country);
    updateField(updates, table__, state);
    updateField(updates, table__, telefone);
    updateField(updates, table__, fax);
    updateField(updates, table__, email);
    updateField(updates, table__, www);
    updateField(updates, table__, language);
    updateField(updates, table__, licensekey);
    updateField(updates, table__, apikey);
    updateField(updates, table__, registered);
    updateField(updates, table__, fileroot);
}
void User::addIDUpdates(Updates& updates) {
}
void User::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Authname);
    ftypes.push_back(Authpass);
    ftypes.push_back(Company);
    ftypes.push_back(Firstname);
    ftypes.push_back(Lastname);
    ftypes.push_back(Street);
    ftypes.push_back(City);
    ftypes.push_back(Zip);
    ftypes.push_back(Country);
    ftypes.push_back(State);
    ftypes.push_back(Telefone);
    ftypes.push_back(Fax);
    ftypes.push_back(Email);
    ftypes.push_back(Www);
    ftypes.push_back(Language);
    ftypes.push_back(Licensekey);
    ftypes.push_back(Apikey);
    ftypes.push_back(Registered);
    ftypes.push_back(Fileroot);
}
void User::delRecord() {
    deleteFromTable(table__, id);
}
void User::delRelations() {
    PresetUserRelation::del(*db, (PresetUserRelation::User == id));
    UserUserGroupRelationUser2UserGroup::del(*db, (UserUserGroupRelationUser2UserGroup::User == id));
}
void User::update() {
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
void User::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<User> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool User::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<User> User::upcast() const {
    return auto_ptr<User>(new User(*this));
}
std::auto_ptr<User> User::upcastCopy() const {
    User* np = new User(*this);
    np->id = id;
    np->type = type;
    np->authname = authname;
    np->authpass = authpass;
    np->company = company;
    np->firstname = firstname;
    np->lastname = lastname;
    np->street = street;
    np->city = city;
    np->zip = zip;
    np->country = country;
    np->state = state;
    np->telefone = telefone;
    np->fax = fax;
    np->email = email;
    np->www = www;
    np->language = language;
    np->licensekey = licensekey;
    np->apikey = apikey;
    np->registered = registered;
    np->fileroot = fileroot;
    np->inDatabase = inDatabase;
    return auto_ptr<User>(np);
}
std::ostream & operator<<(std::ostream& os, User o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.authname.name() << " = " << o.authname << std::endl;
    os << o.authpass.name() << " = " << o.authpass << std::endl;
    os << o.company.name() << " = " << o.company << std::endl;
    os << o.firstname.name() << " = " << o.firstname << std::endl;
    os << o.lastname.name() << " = " << o.lastname << std::endl;
    os << o.street.name() << " = " << o.street << std::endl;
    os << o.city.name() << " = " << o.city << std::endl;
    os << o.zip.name() << " = " << o.zip << std::endl;
    os << o.country.name() << " = " << o.country << std::endl;
    os << o.state.name() << " = " << o.state << std::endl;
    os << o.telefone.name() << " = " << o.telefone << std::endl;
    os << o.fax.name() << " = " << o.fax << std::endl;
    os << o.email.name() << " = " << o.email << std::endl;
    os << o.www.name() << " = " << o.www << std::endl;
    os << o.language.name() << " = " << o.language << std::endl;
    os << o.licensekey.name() << " = " << o.licensekey << std::endl;
    os << o.apikey.name() << " = " << o.apikey << std::endl;
    os << o.registered.name() << " = " << o.registered << std::endl;
    os << o.fileroot.name() << " = " << o.fileroot << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType UserGroup::Own::Id("id_",A_field_type_integer,"UserGroup_");
UserGroup::UserHandle::UserHandle(const UserGroup& owner)
         : litesql::RelationHandle<UserGroup>(owner) {
}
void UserGroup::UserHandle::link(const User& o0) {
    UserUserGroupRelationUser2UserGroup::link(owner->getDatabase(), o0, *owner);
}
void UserGroup::UserHandle::unlink(const User& o0) {
    UserUserGroupRelationUser2UserGroup::unlink(owner->getDatabase(), o0, *owner);
}
void UserGroup::UserHandle::del(const litesql::Expr& expr) {
    UserUserGroupRelationUser2UserGroup::del(owner->getDatabase(), expr && UserUserGroupRelationUser2UserGroup::UserGroup == owner->id);
}
litesql::DataSource<User> UserGroup::UserHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return UserUserGroupRelationUser2UserGroup::get<User>(owner->getDatabase(), expr, (UserUserGroupRelationUser2UserGroup::UserGroup == owner->id) && srcExpr);
}
litesql::DataSource<UserUserGroupRelationUser2UserGroup::Row> UserGroup::UserHandle::getRows(const litesql::Expr& expr) {
    return UserUserGroupRelationUser2UserGroup::getRows(owner->getDatabase(), expr && (UserUserGroupRelationUser2UserGroup::UserGroup == owner->id));
}
const std::string UserGroup::type__("UserGroup");
const std::string UserGroup::table__("UserGroup_");
const std::string UserGroup::sequence__("UserGroup_seq");
const litesql::FieldType UserGroup::Id("id_",A_field_type_integer,table__);
const litesql::FieldType UserGroup::Type("type_",A_field_type_string,table__);
const litesql::FieldType UserGroup::Name("name_",A_field_type_date,table__);
const litesql::FieldType UserGroup::Nodecount("nodecount_",A_field_type_integer,table__);
void UserGroup::defaults() {
    id = 0;
    name = 0;
    nodecount = 0;
}
UserGroup::UserGroup(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name), nodecount(Nodecount) {
    defaults();
}
UserGroup::UserGroup(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name), nodecount(Nodecount) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: nodecount = convert<const std::string&, int>(rec[3]);
        nodecount.setModified(false);
    case 3: name = convert<const std::string&, litesql::Date>(rec[2]);
        name.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
UserGroup::UserGroup(const UserGroup& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name), nodecount(obj.nodecount) {
}
const UserGroup& UserGroup::operator=(const UserGroup& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
        nodecount = obj.nodecount;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
UserGroup::UserHandle UserGroup::user() {
    return UserGroup::UserHandle(*this);
}
std::string UserGroup::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
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
    fields.push_back(nodecount.name());
    values.push_back(nodecount);
    nodecount.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void UserGroup::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void UserGroup::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, name);
    updateField(updates, table__, nodecount);
}
void UserGroup::addIDUpdates(Updates& updates) {
}
void UserGroup::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
    ftypes.push_back(Nodecount);
}
void UserGroup::delRecord() {
    deleteFromTable(table__, id);
}
void UserGroup::delRelations() {
    UserUserGroupRelationUser2UserGroup::del(*db, (UserUserGroupRelationUser2UserGroup::UserGroup == id));
}
void UserGroup::update() {
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
void UserGroup::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<UserGroup> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool UserGroup::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<UserGroup> UserGroup::upcast() const {
    return auto_ptr<UserGroup>(new UserGroup(*this));
}
std::auto_ptr<UserGroup> UserGroup::upcastCopy() const {
    UserGroup* np = new UserGroup(*this);
    np->id = id;
    np->type = type;
    np->name = name;
    np->nodecount = nodecount;
    np->inDatabase = inDatabase;
    return auto_ptr<UserGroup>(np);
}
std::ostream & operator<<(std::ostream& os, UserGroup o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.nodecount.name() << " = " << o.nodecount << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Request::Own::Id("id_",A_field_type_integer,"Request_");
const std::string Request::type__("Request");
const std::string Request::table__("Request_");
const std::string Request::sequence__("Request_seq");
const litesql::FieldType Request::Id("id_",A_field_type_integer,table__);
const litesql::FieldType Request::Type("type_",A_field_type_string,table__);
const litesql::FieldType Request::RequestId("requestId_",A_field_type_string,table__);
const litesql::FieldType Request::RequestType("requestType_",A_field_type_string,table__);
const litesql::FieldType Request::Uri("uri_",A_field_type_string,table__);
const litesql::FieldType Request::Query("query_",A_field_type_string,table__);
const litesql::FieldType Request::Data("data_",A_field_type_string,table__);
const litesql::FieldType Request::Response("response_",A_field_type_string,table__);
void Request::defaults() {
    id = 0;
}
Request::Request(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), requestId(RequestId), requestType(RequestType), uri(Uri), query(Query), data(Data), response(Response) {
    defaults();
}
Request::Request(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), requestId(RequestId), requestType(RequestType), uri(Uri), query(Query), data(Data), response(Response) {
    defaults();
    size_t size = (rec.size() > 8) ? 8 : rec.size();
    switch(size) {
    case 8: response = convert<const std::string&, std::string>(rec[7]);
        response.setModified(false);
    case 7: data = convert<const std::string&, std::string>(rec[6]);
        data.setModified(false);
    case 6: query = convert<const std::string&, std::string>(rec[5]);
        query.setModified(false);
    case 5: uri = convert<const std::string&, std::string>(rec[4]);
        uri.setModified(false);
    case 4: requestType = convert<const std::string&, std::string>(rec[3]);
        requestType.setModified(false);
    case 3: requestId = convert<const std::string&, std::string>(rec[2]);
        requestId.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Request::Request(const Request& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), requestId(obj.requestId), requestType(obj.requestType), uri(obj.uri), query(obj.query), data(obj.data), response(obj.response) {
}
const Request& Request::operator=(const Request& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        requestId = obj.requestId;
        requestType = obj.requestType;
        uri = obj.uri;
        query = obj.query;
        data = obj.data;
        response = obj.response;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string Request::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(requestId.name());
    values.push_back(requestId);
    requestId.setModified(false);
    fields.push_back(requestType.name());
    values.push_back(requestType);
    requestType.setModified(false);
    fields.push_back(uri.name());
    values.push_back(uri);
    uri.setModified(false);
    fields.push_back(query.name());
    values.push_back(query);
    query.setModified(false);
    fields.push_back(data.name());
    values.push_back(data);
    data.setModified(false);
    fields.push_back(response.name());
    values.push_back(response);
    response.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Request::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Request::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, requestId);
    updateField(updates, table__, requestType);
    updateField(updates, table__, uri);
    updateField(updates, table__, query);
    updateField(updates, table__, data);
    updateField(updates, table__, response);
}
void Request::addIDUpdates(Updates& updates) {
}
void Request::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(RequestId);
    ftypes.push_back(RequestType);
    ftypes.push_back(Uri);
    ftypes.push_back(Query);
    ftypes.push_back(Data);
    ftypes.push_back(Response);
}
void Request::delRecord() {
    deleteFromTable(table__, id);
}
void Request::delRelations() {
}
void Request::update() {
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
void Request::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<Request> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Request::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<Request> Request::upcast() const {
    return auto_ptr<Request>(new Request(*this));
}
std::auto_ptr<Request> Request::upcastCopy() const {
    Request* np = new Request(*this);
    np->id = id;
    np->type = type;
    np->requestId = requestId;
    np->requestType = requestType;
    np->uri = uri;
    np->query = query;
    np->data = data;
    np->response = response;
    np->inDatabase = inDatabase;
    return auto_ptr<Request>(np);
}
std::ostream & operator<<(std::ostream& os, Request o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.requestId.name() << " = " << o.requestId << std::endl;
    os << o.requestType.name() << " = " << o.requestType << std::endl;
    os << o.uri.name() << " = " << o.uri << std::endl;
    os << o.query.name() << " = " << o.query << std::endl;
    os << o.data.name() << " = " << o.data << std::endl;
    os << o.response.name() << " = " << o.response << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Partition::Own::Id("id_",A_field_type_integer,"Partition_");
Partition::JobHandle::JobHandle(const Partition& owner)
         : litesql::RelationHandle<Partition>(owner) {
}
void Partition::JobHandle::link(const Job& o0) {
    JobPartitionRelationJob2Partition::link(owner->getDatabase(), o0, *owner);
}
void Partition::JobHandle::unlink(const Job& o0) {
    JobPartitionRelationJob2Partition::unlink(owner->getDatabase(), o0, *owner);
}
void Partition::JobHandle::del(const litesql::Expr& expr) {
    JobPartitionRelationJob2Partition::del(owner->getDatabase(), expr && JobPartitionRelationJob2Partition::Partition == owner->id);
}
litesql::DataSource<Job> Partition::JobHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobPartitionRelationJob2Partition::get<Job>(owner->getDatabase(), expr, (JobPartitionRelationJob2Partition::Partition == owner->id) && srcExpr);
}
litesql::DataSource<JobPartitionRelationJob2Partition::Row> Partition::JobHandle::getRows(const litesql::Expr& expr) {
    return JobPartitionRelationJob2Partition::getRows(owner->getDatabase(), expr && (JobPartitionRelationJob2Partition::Partition == owner->id));
}
const std::string Partition::type__("Partition");
const std::string Partition::table__("Partition_");
const std::string Partition::sequence__("Partition_seq");
const litesql::FieldType Partition::Id("id_",A_field_type_integer,table__);
const litesql::FieldType Partition::Type("type_",A_field_type_string,table__);
const litesql::FieldType Partition::Name("name_",A_field_type_string,table__);
const litesql::FieldType Partition::Partitionsize("partitionsize_",A_field_type_integer,table__);
void Partition::defaults() {
    id = 0;
    partitionsize = 0;
}
Partition::Partition(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name), partitionsize(Partitionsize) {
    defaults();
}
Partition::Partition(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name), partitionsize(Partitionsize) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: partitionsize = convert<const std::string&, int>(rec[3]);
        partitionsize.setModified(false);
    case 3: name = convert<const std::string&, std::string>(rec[2]);
        name.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Partition::Partition(const Partition& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name), partitionsize(obj.partitionsize) {
}
const Partition& Partition::operator=(const Partition& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
        partitionsize = obj.partitionsize;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Partition::JobHandle Partition::job() {
    return Partition::JobHandle(*this);
}
std::string Partition::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
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
    fields.push_back(partitionsize.name());
    values.push_back(partitionsize);
    partitionsize.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Partition::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Partition::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, name);
    updateField(updates, table__, partitionsize);
}
void Partition::addIDUpdates(Updates& updates) {
}
void Partition::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
    ftypes.push_back(Partitionsize);
}
void Partition::delRecord() {
    deleteFromTable(table__, id);
}
void Partition::delRelations() {
    JobPartitionRelationJob2Partition::del(*db, (JobPartitionRelationJob2Partition::Partition == id));
}
void Partition::update() {
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
void Partition::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<Partition> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Partition::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<Partition> Partition::upcast() const {
    return auto_ptr<Partition>(new Partition(*this));
}
std::auto_ptr<Partition> Partition::upcastCopy() const {
    Partition* np = new Partition(*this);
    np->id = id;
    np->type = type;
    np->name = name;
    np->partitionsize = partitionsize;
    np->inDatabase = inDatabase;
    return auto_ptr<Partition>(np);
}
std::ostream & operator<<(std::ostream& os, Partition o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.partitionsize.name() << " = " << o.partitionsize << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Graph::Own::Id("id_",A_field_type_integer,"Graph_");
Graph::TasksHandle::TasksHandle(const Graph& owner)
         : litesql::RelationHandle<Graph>(owner) {
}
void Graph::TasksHandle::link(const Task& o0) {
    GraphTaskRelationGraphTask::link(owner->getDatabase(), *owner, o0);
}
void Graph::TasksHandle::unlink(const Task& o0) {
    GraphTaskRelationGraphTask::unlink(owner->getDatabase(), *owner, o0);
}
void Graph::TasksHandle::del(const litesql::Expr& expr) {
    GraphTaskRelationGraphTask::del(owner->getDatabase(), expr && GraphTaskRelationGraphTask::Graph == owner->id);
}
litesql::DataSource<Task> Graph::TasksHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return GraphTaskRelationGraphTask::get<Task>(owner->getDatabase(), expr, (GraphTaskRelationGraphTask::Graph == owner->id) && srcExpr);
}
litesql::DataSource<GraphTaskRelationGraphTask::Row> Graph::TasksHandle::getRows(const litesql::Expr& expr) {
    return GraphTaskRelationGraphTask::getRows(owner->getDatabase(), expr && (GraphTaskRelationGraphTask::Graph == owner->id));
}
const std::string Graph::type__("Graph");
const std::string Graph::table__("Graph_");
const std::string Graph::sequence__("Graph_seq");
const litesql::FieldType Graph::Id("id_",A_field_type_integer,table__);
const litesql::FieldType Graph::Type("type_",A_field_type_string,table__);
const litesql::FieldType Graph::Uuid("uuid_",A_field_type_string,table__);
const litesql::FieldType Graph::Name("name_",A_field_type_string,table__);
const litesql::FieldType Graph::Data("data_",A_field_type_string,table__);
void Graph::defaults() {
    id = 0;
}
Graph::Graph(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), uuid(Uuid), name(Name), data(Data) {
    defaults();
}
Graph::Graph(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), uuid(Uuid), name(Name), data(Data) {
    defaults();
    size_t size = (rec.size() > 5) ? 5 : rec.size();
    switch(size) {
    case 5: data = convert<const std::string&, std::string>(rec[4]);
        data.setModified(false);
    case 4: name = convert<const std::string&, std::string>(rec[3]);
        name.setModified(false);
    case 3: uuid = convert<const std::string&, std::string>(rec[2]);
        uuid.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Graph::Graph(const Graph& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), uuid(obj.uuid), name(obj.name), data(obj.data) {
}
const Graph& Graph::operator=(const Graph& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        uuid = obj.uuid;
        name = obj.name;
        data = obj.data;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Graph::TasksHandle Graph::tasks() {
    return Graph::TasksHandle(*this);
}
std::string Graph::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(uuid.name());
    values.push_back(uuid);
    uuid.setModified(false);
    fields.push_back(name.name());
    values.push_back(name);
    name.setModified(false);
    fields.push_back(data.name());
    values.push_back(data);
    data.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Graph::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Graph::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, uuid);
    updateField(updates, table__, name);
    updateField(updates, table__, data);
}
void Graph::addIDUpdates(Updates& updates) {
}
void Graph::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Uuid);
    ftypes.push_back(Name);
    ftypes.push_back(Data);
}
void Graph::delRecord() {
    deleteFromTable(table__, id);
}
void Graph::delRelations() {
    GraphTaskRelationGraphTask::del(*db, (GraphTaskRelationGraphTask::Graph == id));
}
void Graph::update() {
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
void Graph::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<Graph> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Graph::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<Graph> Graph::upcast() const {
    return auto_ptr<Graph>(new Graph(*this));
}
std::auto_ptr<Graph> Graph::upcastCopy() const {
    Graph* np = new Graph(*this);
    np->id = id;
    np->type = type;
    np->uuid = uuid;
    np->name = name;
    np->data = data;
    np->inDatabase = inDatabase;
    return auto_ptr<Graph>(np);
}
std::ostream & operator<<(std::ostream& os, Graph o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.uuid.name() << " = " << o.uuid << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.data.name() << " = " << o.data << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Location::Own::Id("id_",A_field_type_integer,"Location_");
const std::string Location::type__("Location");
const std::string Location::table__("Location_");
const std::string Location::sequence__("Location_seq");
const litesql::FieldType Location::Id("id_",A_field_type_integer,table__);
const litesql::FieldType Location::Type("type_",A_field_type_string,table__);
const litesql::FieldType Location::Uuid("uuid_",A_field_type_string,table__);
const litesql::FieldType Location::Name("name_",A_field_type_string,table__);
const litesql::FieldType Location::Data("data_",A_field_type_string,table__);
void Location::defaults() {
    id = 0;
}
Location::Location(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), uuid(Uuid), name(Name), data(Data) {
    defaults();
}
Location::Location(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), uuid(Uuid), name(Name), data(Data) {
    defaults();
    size_t size = (rec.size() > 5) ? 5 : rec.size();
    switch(size) {
    case 5: data = convert<const std::string&, std::string>(rec[4]);
        data.setModified(false);
    case 4: name = convert<const std::string&, std::string>(rec[3]);
        name.setModified(false);
    case 3: uuid = convert<const std::string&, std::string>(rec[2]);
        uuid.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Location::Location(const Location& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), uuid(obj.uuid), name(obj.name), data(obj.data) {
}
const Location& Location::operator=(const Location& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        uuid = obj.uuid;
        name = obj.name;
        data = obj.data;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string Location::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(uuid.name());
    values.push_back(uuid);
    uuid.setModified(false);
    fields.push_back(name.name());
    values.push_back(name);
    name.setModified(false);
    fields.push_back(data.name());
    values.push_back(data);
    data.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Location::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Location::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, uuid);
    updateField(updates, table__, name);
    updateField(updates, table__, data);
}
void Location::addIDUpdates(Updates& updates) {
}
void Location::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Uuid);
    ftypes.push_back(Name);
    ftypes.push_back(Data);
}
void Location::delRecord() {
    deleteFromTable(table__, id);
}
void Location::delRelations() {
}
void Location::update() {
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
void Location::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<Location> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Location::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<Location> Location::upcast() const {
    return auto_ptr<Location>(new Location(*this));
}
std::auto_ptr<Location> Location::upcastCopy() const {
    Location* np = new Location(*this);
    np->id = id;
    np->type = type;
    np->uuid = uuid;
    np->name = name;
    np->data = data;
    np->inDatabase = inDatabase;
    return auto_ptr<Location>(np);
}
std::ostream & operator<<(std::ostream& os, Location o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.uuid.name() << " = " << o.uuid << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.data.name() << " = " << o.data << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Queue::Own::Id("id_",A_field_type_integer,"Queue_");
const int Queue::QtypeType::ONE2ONE(0);
const int Queue::QtypeType::ONE4ALL(1);
Queue::QtypeType::QtypeType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals)
         : litesql::FieldType(n,t,tbl,vals) {
}
const int Queue::Qtype::ONE2ONE(0);
const int Queue::Qtype::ONE4ALL(1);
Queue::JobHandle::JobHandle(const Queue& owner)
         : litesql::RelationHandle<Queue>(owner) {
}
void Queue::JobHandle::link(const Job& o0) {
    JobQueueRelationJob2Queue::link(owner->getDatabase(), o0, *owner);
}
void Queue::JobHandle::unlink(const Job& o0) {
    JobQueueRelationJob2Queue::unlink(owner->getDatabase(), o0, *owner);
}
void Queue::JobHandle::del(const litesql::Expr& expr) {
    JobQueueRelationJob2Queue::del(owner->getDatabase(), expr && JobQueueRelationJob2Queue::Queue == owner->id);
}
litesql::DataSource<Job> Queue::JobHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return JobQueueRelationJob2Queue::get<Job>(owner->getDatabase(), expr, (JobQueueRelationJob2Queue::Queue == owner->id) && srcExpr);
}
litesql::DataSource<JobQueueRelationJob2Queue::Row> Queue::JobHandle::getRows(const litesql::Expr& expr) {
    return JobQueueRelationJob2Queue::getRows(owner->getDatabase(), expr && (JobQueueRelationJob2Queue::Queue == owner->id));
}
const std::string Queue::type__("Queue");
const std::string Queue::table__("Queue_");
const std::string Queue::sequence__("Queue_seq");
const litesql::FieldType Queue::Id("id_",A_field_type_integer,table__);
const litesql::FieldType Queue::Type("type_",A_field_type_string,table__);
const litesql::FieldType Queue::Uuid("uuid_",A_field_type_string,table__);
const litesql::FieldType Queue::Outputname("outputname_",A_field_type_string,table__);
const litesql::FieldType Queue::Inputname("inputname_",A_field_type_string,table__);
std::vector < std::pair< std::string, std::string > > Queue::qtype_values;
const Queue::QtypeType Queue::Qtype("qtype_",A_field_type_integer,table__,qtype_values);
void Queue::initValues() {
    qtype_values.clear();
    qtype_values.push_back(make_pair<std::string, std::string>("ONE2ONE","0"));
    qtype_values.push_back(make_pair<std::string, std::string>("ONE4ALL","1"));
}
void Queue::defaults() {
    id = 0;
    qtype = 0;
}
Queue::Queue(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), uuid(Uuid), outputname(Outputname), inputname(Inputname), qtype(Qtype) {
    defaults();
}
Queue::Queue(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), uuid(Uuid), outputname(Outputname), inputname(Inputname), qtype(Qtype) {
    defaults();
    size_t size = (rec.size() > 6) ? 6 : rec.size();
    switch(size) {
    case 6: qtype = convert<const std::string&, int>(rec[5]);
        qtype.setModified(false);
    case 5: inputname = convert<const std::string&, std::string>(rec[4]);
        inputname.setModified(false);
    case 4: outputname = convert<const std::string&, std::string>(rec[3]);
        outputname.setModified(false);
    case 3: uuid = convert<const std::string&, std::string>(rec[2]);
        uuid.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Queue::Queue(const Queue& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), uuid(obj.uuid), outputname(obj.outputname), inputname(obj.inputname), qtype(obj.qtype) {
}
const Queue& Queue::operator=(const Queue& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        uuid = obj.uuid;
        outputname = obj.outputname;
        inputname = obj.inputname;
        qtype = obj.qtype;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Queue::JobHandle Queue::job() {
    return Queue::JobHandle(*this);
}
std::string Queue::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(uuid.name());
    values.push_back(uuid);
    uuid.setModified(false);
    fields.push_back(outputname.name());
    values.push_back(outputname);
    outputname.setModified(false);
    fields.push_back(inputname.name());
    values.push_back(inputname);
    inputname.setModified(false);
    fields.push_back(qtype.name());
    values.push_back(qtype);
    qtype.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Queue::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Queue::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, uuid);
    updateField(updates, table__, outputname);
    updateField(updates, table__, inputname);
    updateField(updates, table__, qtype);
}
void Queue::addIDUpdates(Updates& updates) {
}
void Queue::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Uuid);
    ftypes.push_back(Outputname);
    ftypes.push_back(Inputname);
    ftypes.push_back(Qtype);
}
void Queue::delRecord() {
    deleteFromTable(table__, id);
}
void Queue::delRelations() {
    JobQueueRelationJob2Queue::del(*db, (JobQueueRelationJob2Queue::Queue == id));
}
void Queue::update() {
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
void Queue::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<Queue> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Queue::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<Queue> Queue::upcast() const {
    return auto_ptr<Queue>(new Queue(*this));
}
std::auto_ptr<Queue> Queue::upcastCopy() const {
    Queue* np = new Queue(*this);
    np->id = id;
    np->type = type;
    np->uuid = uuid;
    np->outputname = outputname;
    np->inputname = inputname;
    np->qtype = qtype;
    np->inDatabase = inDatabase;
    return auto_ptr<Queue>(np);
}
std::ostream & operator<<(std::ostream& os, Queue o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.uuid.name() << " = " << o.uuid << std::endl;
    os << o.outputname.name() << " = " << o.outputname << std::endl;
    os << o.inputname.name() << " = " << o.inputname << std::endl;
    os << o.qtype.name() << " = " << o.qtype << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
HiveDb::HiveDb(std::string backendType, std::string connInfo)
     : litesql::Database(backendType, connInfo) {
    initialize();
}
std::vector<litesql::Database::SchemaItem> HiveDb::getSchema() const {
    vector<Database::SchemaItem> res;
    string TEXT = backend->getSQLType(A_field_type_string);
    string rowIdType = backend->getRowIDType();
    res.push_back(Database::SchemaItem("schema_","table","CREATE TABLE schema_ (name_ "+TEXT+", type_ "+TEXT+", sql_ "+TEXT+")"));
    if (backend->supportsSequences()) {
        res.push_back(Database::SchemaItem("Project_seq","sequence",backend->getCreateSequenceSQL("Project_seq")));
        res.push_back(Database::SchemaItem("Filter_seq","sequence",backend->getCreateSequenceSQL("Filter_seq")));
        res.push_back(Database::SchemaItem("FilterParameter_seq","sequence",backend->getCreateSequenceSQL("FilterParameter_seq")));
        res.push_back(Database::SchemaItem("MediaFile_seq","sequence",backend->getCreateSequenceSQL("MediaFile_seq")));
        res.push_back(Database::SchemaItem("OutputFile_seq","sequence",backend->getCreateSequenceSQL("OutputFile_seq")));
        res.push_back(Database::SchemaItem("ProfileGroup_seq","sequence",backend->getCreateSequenceSQL("ProfileGroup_seq")));
        res.push_back(Database::SchemaItem("Profile_seq","sequence",backend->getCreateSequenceSQL("Profile_seq")));
        res.push_back(Database::SchemaItem("Preset_seq","sequence",backend->getCreateSequenceSQL("Preset_seq")));
        res.push_back(Database::SchemaItem("ProfileParameter_seq","sequence",backend->getCreateSequenceSQL("ProfileParameter_seq")));
        res.push_back(Database::SchemaItem("Stream_seq","sequence",backend->getCreateSequenceSQL("Stream_seq")));
        res.push_back(Database::SchemaItem("StreamParameter_seq","sequence",backend->getCreateSequenceSQL("StreamParameter_seq")));
        res.push_back(Database::SchemaItem("CodecPreset_seq","sequence",backend->getCreateSequenceSQL("CodecPreset_seq")));
        res.push_back(Database::SchemaItem("CodecPresetParameter_seq","sequence",backend->getCreateSequenceSQL("CodecPresetParameter_seq")));
        res.push_back(Database::SchemaItem("Config_seq","sequence",backend->getCreateSequenceSQL("Config_seq")));
        res.push_back(Database::SchemaItem("Job_seq","sequence",backend->getCreateSequenceSQL("Job_seq")));
        res.push_back(Database::SchemaItem("Task_seq","sequence",backend->getCreateSequenceSQL("Task_seq")));
        res.push_back(Database::SchemaItem("JobLog_seq","sequence",backend->getCreateSequenceSQL("JobLog_seq")));
        res.push_back(Database::SchemaItem("JobDetail_seq","sequence",backend->getCreateSequenceSQL("JobDetail_seq")));
        res.push_back(Database::SchemaItem("Watchfolder_seq","sequence",backend->getCreateSequenceSQL("Watchfolder_seq")));
        res.push_back(Database::SchemaItem("Process_seq","sequence",backend->getCreateSequenceSQL("Process_seq")));
        res.push_back(Database::SchemaItem("ProcessUnit_seq","sequence",backend->getCreateSequenceSQL("ProcessUnit_seq")));
        res.push_back(Database::SchemaItem("User_seq","sequence",backend->getCreateSequenceSQL("User_seq")));
        res.push_back(Database::SchemaItem("UserGroup_seq","sequence",backend->getCreateSequenceSQL("UserGroup_seq")));
        res.push_back(Database::SchemaItem("Request_seq","sequence",backend->getCreateSequenceSQL("Request_seq")));
        res.push_back(Database::SchemaItem("Partition_seq","sequence",backend->getCreateSequenceSQL("Partition_seq")));
        res.push_back(Database::SchemaItem("Graph_seq","sequence",backend->getCreateSequenceSQL("Graph_seq")));
        res.push_back(Database::SchemaItem("Location_seq","sequence",backend->getCreateSequenceSQL("Location_seq")));
        res.push_back(Database::SchemaItem("Queue_seq","sequence",backend->getCreateSequenceSQL("Queue_seq")));
    }
    res.push_back(Database::SchemaItem("Project_","table","CREATE TABLE Project_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",name_ " + backend->getSQLType(A_field_type_string,"") + "" +",outdirectory_ " + backend->getSQLType(A_field_type_string,"") + "" +",status_ " + backend->getSQLType(A_field_type_string,"") + "" +",created_ " + backend->getSQLType(A_field_type_date,"") + "" +",started_ " + backend->getSQLType(A_field_type_date,"") + "" +",completed_ " + backend->getSQLType(A_field_type_date,"") + "" +")"));
    res.push_back(Database::SchemaItem("Filter_","table","CREATE TABLE Filter_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",filtername_ " + backend->getSQLType(A_field_type_string,"") + "" +",filterid_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("FilterParameter_","table","CREATE TABLE FilterParameter_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",fkey_ " + backend->getSQLType(A_field_type_string,"") + "" +",fval_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("MediaFile_","table","CREATE TABLE MediaFile_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",filename_ " + backend->getSQLType(A_field_type_string,"") + "" +",path_ " + backend->getSQLType(A_field_type_string,"") + "" +",filesize_ " + backend->getSQLType(A_field_type_double,"") + "" +",streamcount_ " + backend->getSQLType(A_field_type_integer,"") + "" +",containertype_ " + backend->getSQLType(A_field_type_string,"") + "" +",duration_ " + backend->getSQLType(A_field_type_double,"") + "" +",starttime_ " + backend->getSQLType(A_field_type_double,"") + "" +",bitrate_ " + backend->getSQLType(A_field_type_integer,"") + "" +",created_ " + backend->getSQLType(A_field_type_datetime,"") + "" +",filetype_ " + backend->getSQLType(A_field_type_integer,"") + "" +",parent_ " + backend->getSQLType(A_field_type_integer,"") + "" +",metatitle_ " + backend->getSQLType(A_field_type_string,"") + "" +",metaauthor_ " + backend->getSQLType(A_field_type_string,"") + "" +",metacopyright_ " + backend->getSQLType(A_field_type_string,"") + "" +",metacomment_ " + backend->getSQLType(A_field_type_string,"") + "" +",metaalbum_ " + backend->getSQLType(A_field_type_string,"") + "" +",metayear_ " + backend->getSQLType(A_field_type_integer,"") + "" +",metatrack_ " + backend->getSQLType(A_field_type_integer,"") + "" +",metagenre_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("OutputFile_","table","CREATE TABLE OutputFile_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",filename_ " + backend->getSQLType(A_field_type_string,"") + "" +",path_ " + backend->getSQLType(A_field_type_string,"") + "" +",jobid_ " + backend->getSQLType(A_field_type_string,"") + "" +",outfiledata_ " + backend->getSQLType(A_field_type_string,"") + "" +",status_ " + backend->getSQLType(A_field_type_string,"") + "" +",uuid_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("ProfileGroup_","table","CREATE TABLE ProfileGroup_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",name_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("Profile_","table","CREATE TABLE Profile_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",name_ " + backend->getSQLType(A_field_type_string,"") + "" +",created_ " + backend->getSQLType(A_field_type_datetime,"") + "" +",format_ " + backend->getSQLType(A_field_type_string,"") + "" +",formatext_ " + backend->getSQLType(A_field_type_string,"") + "" +",vcodec_ " + backend->getSQLType(A_field_type_integer,"") + "" +",vbitrate_ " + backend->getSQLType(A_field_type_integer,"") + "" +",vframerate_ " + backend->getSQLType(A_field_type_string,"") + "" +",vwidth_ " + backend->getSQLType(A_field_type_integer,"") + "" +",vheight_ " + backend->getSQLType(A_field_type_integer,"") + "" +",vextra_ " + backend->getSQLType(A_field_type_string,"") + "" +",achannels_ " + backend->getSQLType(A_field_type_integer,"") + "" +",acodec_ " + backend->getSQLType(A_field_type_integer,"") + "" +",abitrate_ " + backend->getSQLType(A_field_type_integer,"") + "" +",asamplerate_ " + backend->getSQLType(A_field_type_integer,"") + "" +",aextra_ " + backend->getSQLType(A_field_type_string,"") + "" +",profiletype_ " + backend->getSQLType(A_field_type_integer,"") + "" +",deinterlace_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Preset_","table","CREATE TABLE Preset_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",uuid_ " + backend->getSQLType(A_field_type_string,"") + "" +",name_ " + backend->getSQLType(A_field_type_string,"") + "" +",filename_ " + backend->getSQLType(A_field_type_string,"") + "" +",data_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("ProfileParameter_","table","CREATE TABLE ProfileParameter_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",name_ " + backend->getSQLType(A_field_type_string,"") + "" +",val_ " + backend->getSQLType(A_field_type_string,"") + "" +",mediatype_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Stream_","table","CREATE TABLE Stream_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",streamindex_ " + backend->getSQLType(A_field_type_integer,"") + "" +",streamtype_ " + backend->getSQLType(A_field_type_integer,"") + "" +",codecid_ " + backend->getSQLType(A_field_type_integer,"") + "" +",codecname_ " + backend->getSQLType(A_field_type_string,"") + "" +",frameratenum_ " + backend->getSQLType(A_field_type_integer,"") + "" +",framerateden_ " + backend->getSQLType(A_field_type_integer,"") + "" +",streamtimebasenum_ " + backend->getSQLType(A_field_type_integer,"") + "" +",streamtimebaseden_ " + backend->getSQLType(A_field_type_integer,"") + "" +",codectimebasenum_ " + backend->getSQLType(A_field_type_integer,"") + "" +",codectimebaseden_ " + backend->getSQLType(A_field_type_integer,"") + "" +",firstpts_ " + backend->getSQLType(A_field_type_double,"") + "" +",firstdts_ " + backend->getSQLType(A_field_type_double,"") + "" +",duration_ " + backend->getSQLType(A_field_type_double,"") + "" +",nbframes_ " + backend->getSQLType(A_field_type_double,"") + "" +",ticksperframe_ " + backend->getSQLType(A_field_type_integer,"") + "" +",framecount_ " + backend->getSQLType(A_field_type_integer,"") + "" +",width_ " + backend->getSQLType(A_field_type_integer,"") + "" +",height_ " + backend->getSQLType(A_field_type_integer,"") + "" +",gopsize_ " + backend->getSQLType(A_field_type_integer,"") + "" +",pixfmt_ " + backend->getSQLType(A_field_type_integer,"") + "" +",bitrate_ " + backend->getSQLType(A_field_type_integer,"") + "" +",samplerate_ " + backend->getSQLType(A_field_type_integer,"") + "" +",samplefmt_ " + backend->getSQLType(A_field_type_integer,"") + "" +",channels_ " + backend->getSQLType(A_field_type_integer,"") + "" +",bitspercodedsample_ " + backend->getSQLType(A_field_type_integer,"") + "" +",privdatasize_ " + backend->getSQLType(A_field_type_integer,"") + "" +",privdata_ " + backend->getSQLType(A_field_type_string,"") + "" +",extradatasize_ " + backend->getSQLType(A_field_type_integer,"") + "" +",extradata_ " + backend->getSQLType(A_field_type_blob,"") + "" +",aspectratio_ " + backend->getSQLType(A_field_type_string,"") + "" +",flags_ " + backend->getSQLType(A_field_type_integer,"") + "" +",extraprofileflags_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("StreamParameter_","table","CREATE TABLE StreamParameter_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",name_ " + backend->getSQLType(A_field_type_string,"") + "" +",val_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("CodecPreset_","table","CREATE TABLE CodecPreset_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",name_ " + backend->getSQLType(A_field_type_string,"") + "" +",created_ " + backend->getSQLType(A_field_type_date,"") + "" +",codecid_ " + backend->getSQLType(A_field_type_integer,"") + "" +",preset_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("CodecPresetParameter_","table","CREATE TABLE CodecPresetParameter_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",name_ " + backend->getSQLType(A_field_type_string,"") + "" +",val_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("Config_","table","CREATE TABLE Config_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",configkey_ " + backend->getSQLType(A_field_type_string,"") + "" +",configval_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("Job_","table","CREATE TABLE Job_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",uuid_ " + backend->getSQLType(A_field_type_string,"") + "" +",created_ " + backend->getSQLType(A_field_type_datetime,"") + "" +",begintime_ " + backend->getSQLType(A_field_type_datetime,"") + "" +",endtime_ " + backend->getSQLType(A_field_type_datetime,"") + "" +",status_ " + backend->getSQLType(A_field_type_string,"") + "" +",infile_ " + backend->getSQLType(A_field_type_string,"") + "" +",outfile_ " + backend->getSQLType(A_field_type_string,"") + "" +",starttime_ " + backend->getSQLType(A_field_type_double,"") + "" +",duration_ " + backend->getSQLType(A_field_type_double,"") + "" +",progress_ " + backend->getSQLType(A_field_type_integer,"") + "" +",fps_ " + backend->getSQLType(A_field_type_integer,"") + "" +",data_ " + backend->getSQLType(A_field_type_string,"") + "" +",deleted_ " + backend->getSQLType(A_field_type_integer,"") + "" +",partitionname_ " + backend->getSQLType(A_field_type_string,"") + "" +",graph_ " + backend->getSQLType(A_field_type_string,"") + "" +",graphstatus_ " + backend->getSQLType(A_field_type_string,"") + "" +",graphname_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("Task_","table","CREATE TABLE Task_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",uuid_ " + backend->getSQLType(A_field_type_string,"") + "" +",name_ " + backend->getSQLType(A_field_type_string,"") + "" +",parameter_ " + backend->getSQLType(A_field_type_string,"") + "" +",statustext_ " + backend->getSQLType(A_field_type_string,"") + "" +",progress_ " + backend->getSQLType(A_field_type_integer,"") + "" +",status_ " + backend->getSQLType(A_field_type_integer,"") + "" +",sortorder_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("JobLog_","table","CREATE TABLE JobLog_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",created_ " + backend->getSQLType(A_field_type_datetime,"") + "" +",message_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("JobDetail_","table","CREATE TABLE JobDetail_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",lastpts_ " + backend->getSQLType(A_field_type_double,"") + "" +",lastdts_ " + backend->getSQLType(A_field_type_double,"") + "" +",deinterlace_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Watchfolder_","table","CREATE TABLE Watchfolder_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",folder_ " + backend->getSQLType(A_field_type_string,"") + "" +",filefilter_ " + backend->getSQLType(A_field_type_string,"") + "" +",subdirs_ " + backend->getSQLType(A_field_type_integer,"") + "" +",flowuuid_ " + backend->getSQLType(A_field_type_string,"") + "" +",interval_ " + backend->getSQLType(A_field_type_integer,"") + "" +",start_ " + backend->getSQLType(A_field_type_datetime,"") + "" +",end_ " + backend->getSQLType(A_field_type_datetime,"") + "" +")"));
    res.push_back(Database::SchemaItem("Process_","table","CREATE TABLE Process_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",executable_ " + backend->getSQLType(A_field_type_string,"") + "" +",arguments_ " + backend->getSQLType(A_field_type_string,"") + "" +",name_ " + backend->getSQLType(A_field_type_string,"") + "" +",pid_ " + backend->getSQLType(A_field_type_integer,"") + "" +",started_ " + backend->getSQLType(A_field_type_datetime,"") + "" +",stopped_ " + backend->getSQLType(A_field_type_datetime,"") + "" +")"));
    res.push_back(Database::SchemaItem("ProcessUnit_","table","CREATE TABLE ProcessUnit_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",sorcestream_ " + backend->getSQLType(A_field_type_integer,"") + "" +",targetstream_ " + backend->getSQLType(A_field_type_integer,"") + "" +",timebasenum_ " + backend->getSQLType(A_field_type_integer,"") + "" +",timebaseden_ " + backend->getSQLType(A_field_type_integer,"") + "" +",startts_ " + backend->getSQLType(A_field_type_double,"") + "" +",endts_ " + backend->getSQLType(A_field_type_double,"") + "" +",framecount_ " + backend->getSQLType(A_field_type_integer,"") + "" +",send_ " + backend->getSQLType(A_field_type_datetime,"") + "" +",recv_ " + backend->getSQLType(A_field_type_datetime,"") + "" +",sendid_ " + backend->getSQLType(A_field_type_string,"") + "" +",recvid_ " + backend->getSQLType(A_field_type_string,"") + "" +",data_ " + backend->getSQLType(A_field_type_blob,"") + "" +",responseData_ " + backend->getSQLType(A_field_type_blob,"") + "" +",clientid_ " + backend->getSQLType(A_field_type_string,"") + "" +",deliverycount_ " + backend->getSQLType(A_field_type_integer,"") + "" +",jobid_ " + backend->getSQLType(A_field_type_string,"") + "" +",group_ " + backend->getSQLType(A_field_type_string,"") + "" +",sequence_ " + backend->getSQLType(A_field_type_integer,"") + "" +",codectype_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("User_","table","CREATE TABLE User_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",authname_ " + backend->getSQLType(A_field_type_string,"") + "" +",authpass_ " + backend->getSQLType(A_field_type_string,"") + "" +",company_ " + backend->getSQLType(A_field_type_string,"") + "" +",firstname_ " + backend->getSQLType(A_field_type_string,"") + "" +",lastname_ " + backend->getSQLType(A_field_type_string,"") + "" +",street_ " + backend->getSQLType(A_field_type_string,"") + "" +",city_ " + backend->getSQLType(A_field_type_string,"") + "" +",zip_ " + backend->getSQLType(A_field_type_string,"") + "" +",country_ " + backend->getSQLType(A_field_type_string,"") + "" +",state_ " + backend->getSQLType(A_field_type_string,"") + "" +",telefone_ " + backend->getSQLType(A_field_type_string,"") + "" +",fax_ " + backend->getSQLType(A_field_type_string,"") + "" +",email_ " + backend->getSQLType(A_field_type_string,"") + "" +",www_ " + backend->getSQLType(A_field_type_string,"") + "" +",language_ " + backend->getSQLType(A_field_type_string,"") + "" +",licensekey_ " + backend->getSQLType(A_field_type_string,"") + "" +",apikey_ " + backend->getSQLType(A_field_type_string,"") + "" +",registered_ " + backend->getSQLType(A_field_type_date,"") + "" +",fileroot_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("UserGroup_","table","CREATE TABLE UserGroup_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",name_ " + backend->getSQLType(A_field_type_date,"") + "" +",nodecount_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Request_","table","CREATE TABLE Request_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",requestId_ " + backend->getSQLType(A_field_type_string,"") + "" +",requestType_ " + backend->getSQLType(A_field_type_string,"") + "" +",uri_ " + backend->getSQLType(A_field_type_string,"") + "" +",query_ " + backend->getSQLType(A_field_type_string,"") + "" +",data_ " + backend->getSQLType(A_field_type_string,"") + "" +",response_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("Partition_","table","CREATE TABLE Partition_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",name_ " + backend->getSQLType(A_field_type_string,"") + "" +",partitionsize_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Graph_","table","CREATE TABLE Graph_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",uuid_ " + backend->getSQLType(A_field_type_string,"") + "" +",name_ " + backend->getSQLType(A_field_type_string,"") + "" +",data_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("Location_","table","CREATE TABLE Location_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",uuid_ " + backend->getSQLType(A_field_type_string,"") + "" +",name_ " + backend->getSQLType(A_field_type_string,"") + "" +",data_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("Queue_","table","CREATE TABLE Queue_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",uuid_ " + backend->getSQLType(A_field_type_string,"") + "" +",outputname_ " + backend->getSQLType(A_field_type_string,"") + "" +",inputname_ " + backend->getSQLType(A_field_type_string,"") + "" +",qtype_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Filter_FilterParameter_","table","CREATE TABLE Filter_FilterParameter_ (Filter1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",FilterParameter2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Filter_MediaFile_","table","CREATE TABLE Filter_MediaFile_ (Filter1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",MediaFile2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Filter_Project_","table","CREATE TABLE Filter_Project_ (Filter1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Project2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("MediaFile_Project_","table","CREATE TABLE MediaFile_Project_ (MediaFile1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Project2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Preset_Project_","table","CREATE TABLE Preset_Project_ (Preset1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Project2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Preset_User_","table","CREATE TABLE Preset_User_ (Preset1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",User2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Filter_Profile_","table","CREATE TABLE Filter_Profile_ (Filter1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Profile2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("O60a643d384d1f6c6ddeba5bb8ac0f","table","CREATE TABLE O60a643d384d1f6c6ddeba5bb8ac0f (Profile1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",ProfileGroup2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("O2d57c481daf84ed6d04cd9e705469","table","CREATE TABLE O2d57c481daf84ed6d04cd9e705469 (ProfileGroup1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",ProfileGroup2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Profile_ProfileParameter_","table","CREATE TABLE Profile_ProfileParameter_ (Profile1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",ProfileParameter2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Profile_Project_","table","CREATE TABLE Profile_Project_ (Profile1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Project2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Stream_StreamParameter_","table","CREATE TABLE Stream_StreamParameter_ (Stream1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",StreamParameter2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Filter_Stream_","table","CREATE TABLE Filter_Stream_ (Filter1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Stream2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("MediaFile_Stream_","table","CREATE TABLE MediaFile_Stream_ (MediaFile1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Stream2_ " + backend->getSQLType(A_field_type_integer,"") + " UNIQUE" +")"));
    res.push_back(Database::SchemaItem("O165bce89be0b4f99d8ddeba7a26a2","table","CREATE TABLE O165bce89be0b4f99d8ddeba7a26a2 (CodecPreset1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",CodecPresetParameter2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Ob477e426317c3764439827c70cd95","table","CREATE TABLE Ob477e426317c3764439827c70cd95 (CodecPreset1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Profile2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Oc47426250800c92cff81a427efb64","table","CREATE TABLE Oc47426250800c92cff81a427efb64 (CodecPreset1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Profile2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Job_Task_JobTask","table","CREATE TABLE Job_Task_JobTask (Job1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Task2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Graph_Task_GraphTask","table","CREATE TABLE Graph_Task_GraphTask (Graph1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Task2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Job_JobLog_JobJobLog","table","CREATE TABLE Job_JobLog_JobJobLog (Job1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",JobLog2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Job_MediaFile_JobInFile","table","CREATE TABLE Job_MediaFile_JobInFile (Job1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",MediaFile2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Job_MediaFile_JobOutFile","table","CREATE TABLE Job_MediaFile_JobOutFile (Job1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",MediaFile2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Job_Preset_","table","CREATE TABLE Job_Preset_ (Job1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Preset2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Job_JobDetail_JobJobDetail","table","CREATE TABLE Job_JobDetail_JobJobDetail (Job1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",JobDetail2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("JobDetail_Stream_JobOutStream","table","CREATE TABLE JobDetail_Stream_JobOutStream (JobDetail1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Stream2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("JobDetail_Stream_JobInStream","table","CREATE TABLE JobDetail_Stream_JobInStream (JobDetail1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Stream2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Job_ProcessUnit_ProcessUnitJob","table","CREATE TABLE Job_ProcessUnit_ProcessUnitJob (Job1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",ProcessUnit2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("User_UserGroup_User2UserGroup","table","CREATE TABLE User_UserGroup_User2UserGroup (User1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",UserGroup2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Job_Partition_Job2Partition","table","CREATE TABLE Job_Partition_Job2Partition (Job1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Partition2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Job_Queue_Job2Queue","table","CREATE TABLE Job_Queue_Job2Queue (Job1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Queue2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Project_id_idx","index","CREATE INDEX Project_id_idx ON Project_ (id_)"));
    res.push_back(Database::SchemaItem("Filter_id_idx","index","CREATE INDEX Filter_id_idx ON Filter_ (id_)"));
    res.push_back(Database::SchemaItem("FilterParameter_id_idx","index","CREATE INDEX FilterParameter_id_idx ON FilterParameter_ (id_)"));
    res.push_back(Database::SchemaItem("MediaFile_id_idx","index","CREATE INDEX MediaFile_id_idx ON MediaFile_ (id_)"));
    res.push_back(Database::SchemaItem("OutputFile_id_idx","index","CREATE INDEX OutputFile_id_idx ON OutputFile_ (id_)"));
    res.push_back(Database::SchemaItem("OutputFile_status_idx","index","CREATE INDEX OutputFile_status_idx ON OutputFile_ (status_)"));
    res.push_back(Database::SchemaItem("ProfileGroup_id_idx","index","CREATE INDEX ProfileGroup_id_idx ON ProfileGroup_ (id_)"));
    res.push_back(Database::SchemaItem("Profile_id_idx","index","CREATE INDEX Profile_id_idx ON Profile_ (id_)"));
    res.push_back(Database::SchemaItem("Preset_id_idx","index","CREATE INDEX Preset_id_idx ON Preset_ (id_)"));
    res.push_back(Database::SchemaItem("ProfileParameter_id_idx","index","CREATE INDEX ProfileParameter_id_idx ON ProfileParameter_ (id_)"));
    res.push_back(Database::SchemaItem("Stream_id_idx","index","CREATE INDEX Stream_id_idx ON Stream_ (id_)"));
    res.push_back(Database::SchemaItem("StreamParameter_id_idx","index","CREATE INDEX StreamParameter_id_idx ON StreamParameter_ (id_)"));
    res.push_back(Database::SchemaItem("CodecPreset_id_idx","index","CREATE INDEX CodecPreset_id_idx ON CodecPreset_ (id_)"));
    res.push_back(Database::SchemaItem("CodecPresetParameter_id_idx","index","CREATE INDEX CodecPresetParameter_id_idx ON CodecPresetParameter_ (id_)"));
    res.push_back(Database::SchemaItem("Config_id_idx","index","CREATE INDEX Config_id_idx ON Config_ (id_)"));
    res.push_back(Database::SchemaItem("Job_id_idx","index","CREATE INDEX Job_id_idx ON Job_ (id_)"));
    res.push_back(Database::SchemaItem("Task_id_idx","index","CREATE INDEX Task_id_idx ON Task_ (id_)"));
    res.push_back(Database::SchemaItem("JobLog_id_idx","index","CREATE INDEX JobLog_id_idx ON JobLog_ (id_)"));
    res.push_back(Database::SchemaItem("JobDetail_id_idx","index","CREATE INDEX JobDetail_id_idx ON JobDetail_ (id_)"));
    res.push_back(Database::SchemaItem("Watchfolder_id_idx","index","CREATE INDEX Watchfolder_id_idx ON Watchfolder_ (id_)"));
    res.push_back(Database::SchemaItem("Process_id_idx","index","CREATE INDEX Process_id_idx ON Process_ (id_)"));
    res.push_back(Database::SchemaItem("ProcessUnit_id_idx","index","CREATE INDEX ProcessUnit_id_idx ON ProcessUnit_ (id_)"));
    res.push_back(Database::SchemaItem("ProcessUnit_send_idx","index","CREATE INDEX ProcessUnit_send_idx ON ProcessUnit_ (send_)"));
    res.push_back(Database::SchemaItem("ProcessUnit_recv_idx","index","CREATE INDEX ProcessUnit_recv_idx ON ProcessUnit_ (recv_)"));
    res.push_back(Database::SchemaItem("ProcessUnit_jobid_idx","index","CREATE INDEX ProcessUnit_jobid_idx ON ProcessUnit_ (jobid_)"));
    res.push_back(Database::SchemaItem("User_id_idx","index","CREATE INDEX User_id_idx ON User_ (id_)"));
    res.push_back(Database::SchemaItem("UserGroup_id_idx","index","CREATE INDEX UserGroup_id_idx ON UserGroup_ (id_)"));
    res.push_back(Database::SchemaItem("Request_id_idx","index","CREATE INDEX Request_id_idx ON Request_ (id_)"));
    res.push_back(Database::SchemaItem("Partition_id_idx","index","CREATE INDEX Partition_id_idx ON Partition_ (id_)"));
    res.push_back(Database::SchemaItem("Graph_id_idx","index","CREATE INDEX Graph_id_idx ON Graph_ (id_)"));
    res.push_back(Database::SchemaItem("Location_id_idx","index","CREATE INDEX Location_id_idx ON Location_ (id_)"));
    res.push_back(Database::SchemaItem("Queue_id_idx","index","CREATE INDEX Queue_id_idx ON Queue_ (id_)"));
    res.push_back(Database::SchemaItem("O31bc02ae6378692311b53a67f5827","index","CREATE INDEX O31bc02ae6378692311b53a67f5827 ON Filter_FilterParameter_ (Filter1_)"));
    res.push_back(Database::SchemaItem("O2014ef88d528f2929ec192b395d91","index","CREATE INDEX O2014ef88d528f2929ec192b395d91 ON Filter_FilterParameter_ (FilterParameter2_)"));
    res.push_back(Database::SchemaItem("O856202d949b3ed9064656a27f198c","index","CREATE INDEX O856202d949b3ed9064656a27f198c ON Filter_FilterParameter_ (Filter1_,FilterParameter2_)"));
    res.push_back(Database::SchemaItem("Filter_MediaFile_Filter1_idx","index","CREATE INDEX Filter_MediaFile_Filter1_idx ON Filter_MediaFile_ (Filter1_)"));
    res.push_back(Database::SchemaItem("Od0b73dbf04f278921d4d9aca86fe1","index","CREATE INDEX Od0b73dbf04f278921d4d9aca86fe1 ON Filter_MediaFile_ (MediaFile2_)"));
    res.push_back(Database::SchemaItem("Filter_MediaFile__all_idx","index","CREATE INDEX Filter_MediaFile__all_idx ON Filter_MediaFile_ (Filter1_,MediaFile2_)"));
    res.push_back(Database::SchemaItem("Filter_Project_Filter1_idx","index","CREATE INDEX Filter_Project_Filter1_idx ON Filter_Project_ (Filter1_)"));
    res.push_back(Database::SchemaItem("Filter_Project_Project2_idx","index","CREATE INDEX Filter_Project_Project2_idx ON Filter_Project_ (Project2_)"));
    res.push_back(Database::SchemaItem("Filter_Project__all_idx","index","CREATE INDEX Filter_Project__all_idx ON Filter_Project_ (Filter1_,Project2_)"));
    res.push_back(Database::SchemaItem("O0d98700600d7f936bfc0a9815149f","index","CREATE INDEX O0d98700600d7f936bfc0a9815149f ON MediaFile_Project_ (MediaFile1_)"));
    res.push_back(Database::SchemaItem("MediaFile_Project_Project2_idx","index","CREATE INDEX MediaFile_Project_Project2_idx ON MediaFile_Project_ (Project2_)"));
    res.push_back(Database::SchemaItem("MediaFile_Project__all_idx","index","CREATE INDEX MediaFile_Project__all_idx ON MediaFile_Project_ (MediaFile1_,Project2_)"));
    res.push_back(Database::SchemaItem("Preset_Project_Preset1_idx","index","CREATE INDEX Preset_Project_Preset1_idx ON Preset_Project_ (Preset1_)"));
    res.push_back(Database::SchemaItem("Preset_Project_Project2_idx","index","CREATE INDEX Preset_Project_Project2_idx ON Preset_Project_ (Project2_)"));
    res.push_back(Database::SchemaItem("Preset_Project__all_idx","index","CREATE INDEX Preset_Project__all_idx ON Preset_Project_ (Preset1_,Project2_)"));
    res.push_back(Database::SchemaItem("Preset_User_Preset1_idx","index","CREATE INDEX Preset_User_Preset1_idx ON Preset_User_ (Preset1_)"));
    res.push_back(Database::SchemaItem("Preset_User_User2_idx","index","CREATE INDEX Preset_User_User2_idx ON Preset_User_ (User2_)"));
    res.push_back(Database::SchemaItem("Preset_User__all_idx","index","CREATE INDEX Preset_User__all_idx ON Preset_User_ (Preset1_,User2_)"));
    res.push_back(Database::SchemaItem("Filter_Profile_Filter1_idx","index","CREATE INDEX Filter_Profile_Filter1_idx ON Filter_Profile_ (Filter1_)"));
    res.push_back(Database::SchemaItem("Filter_Profile_Profile2_idx","index","CREATE INDEX Filter_Profile_Profile2_idx ON Filter_Profile_ (Profile2_)"));
    res.push_back(Database::SchemaItem("Filter_Profile__all_idx","index","CREATE INDEX Filter_Profile__all_idx ON Filter_Profile_ (Filter1_,Profile2_)"));
    res.push_back(Database::SchemaItem("O79737c1f5eb08516d34194787512a","index","CREATE INDEX O79737c1f5eb08516d34194787512a ON O60a643d384d1f6c6ddeba5bb8ac0f (Profile1_)"));
    res.push_back(Database::SchemaItem("O8a50cd09b66a24a77724c5b5b8262","index","CREATE INDEX O8a50cd09b66a24a77724c5b5b8262 ON O60a643d384d1f6c6ddeba5bb8ac0f (ProfileGroup2_)"));
    res.push_back(Database::SchemaItem("Of33741e78c92c054fb5c1f1d26259","index","CREATE INDEX Of33741e78c92c054fb5c1f1d26259 ON O60a643d384d1f6c6ddeba5bb8ac0f (Profile1_,ProfileGroup2_)"));
    res.push_back(Database::SchemaItem("Obe205b2098fadc7bc7c404386eed4","index","CREATE INDEX Obe205b2098fadc7bc7c404386eed4 ON O2d57c481daf84ed6d04cd9e705469 (ProfileGroup1_)"));
    res.push_back(Database::SchemaItem("O01c0d590e61409de75847deaaff42","index","CREATE INDEX O01c0d590e61409de75847deaaff42 ON O2d57c481daf84ed6d04cd9e705469 (ProfileGroup2_)"));
    res.push_back(Database::SchemaItem("O87743db8b7790aa5011db7d78c064","index","CREATE INDEX O87743db8b7790aa5011db7d78c064 ON O2d57c481daf84ed6d04cd9e705469 (ProfileGroup1_,ProfileGroup2_)"));
    res.push_back(Database::SchemaItem("O1e2bfddca9fca28b91deba705a874","index","CREATE INDEX O1e2bfddca9fca28b91deba705a874 ON Profile_ProfileParameter_ (Profile1_)"));
    res.push_back(Database::SchemaItem("O82775ff79ddf9bab85e7f6a49b600","index","CREATE INDEX O82775ff79ddf9bab85e7f6a49b600 ON Profile_ProfileParameter_ (ProfileParameter2_)"));
    res.push_back(Database::SchemaItem("O511e5af2def498717dc8291922087","index","CREATE INDEX O511e5af2def498717dc8291922087 ON Profile_ProfileParameter_ (Profile1_,ProfileParameter2_)"));
    res.push_back(Database::SchemaItem("Profile_Project_Profile1_idx","index","CREATE INDEX Profile_Project_Profile1_idx ON Profile_Project_ (Profile1_)"));
    res.push_back(Database::SchemaItem("Profile_Project_Project2_idx","index","CREATE INDEX Profile_Project_Project2_idx ON Profile_Project_ (Project2_)"));
    res.push_back(Database::SchemaItem("Profile_Project__all_idx","index","CREATE INDEX Profile_Project__all_idx ON Profile_Project_ (Profile1_,Project2_)"));
    res.push_back(Database::SchemaItem("O3db941b2c1bdd66bc4b5a7340076f","index","CREATE INDEX O3db941b2c1bdd66bc4b5a7340076f ON Stream_StreamParameter_ (Stream1_)"));
    res.push_back(Database::SchemaItem("O673d2f8a05604d79a15b24dbd5ba3","index","CREATE INDEX O673d2f8a05604d79a15b24dbd5ba3 ON Stream_StreamParameter_ (StreamParameter2_)"));
    res.push_back(Database::SchemaItem("O65e9a986eede10413106be0835411","index","CREATE INDEX O65e9a986eede10413106be0835411 ON Stream_StreamParameter_ (Stream1_,StreamParameter2_)"));
    res.push_back(Database::SchemaItem("Filter_Stream_Filter1_idx","index","CREATE INDEX Filter_Stream_Filter1_idx ON Filter_Stream_ (Filter1_)"));
    res.push_back(Database::SchemaItem("Filter_Stream_Stream2_idx","index","CREATE INDEX Filter_Stream_Stream2_idx ON Filter_Stream_ (Stream2_)"));
    res.push_back(Database::SchemaItem("Filter_Stream__all_idx","index","CREATE INDEX Filter_Stream__all_idx ON Filter_Stream_ (Filter1_,Stream2_)"));
    res.push_back(Database::SchemaItem("O59c35c10c26768c7f59083424f1cd","index","CREATE INDEX O59c35c10c26768c7f59083424f1cd ON MediaFile_Stream_ (MediaFile1_)"));
    res.push_back(Database::SchemaItem("MediaFile_Stream_Stream2_idx","index","CREATE INDEX MediaFile_Stream_Stream2_idx ON MediaFile_Stream_ (Stream2_)"));
    res.push_back(Database::SchemaItem("MediaFile_Stream__all_idx","index","CREATE INDEX MediaFile_Stream__all_idx ON MediaFile_Stream_ (MediaFile1_,Stream2_)"));
    res.push_back(Database::SchemaItem("O55446a480dd7ff767b098761a6704","index","CREATE INDEX O55446a480dd7ff767b098761a6704 ON O165bce89be0b4f99d8ddeba7a26a2 (CodecPreset1_)"));
    res.push_back(Database::SchemaItem("Ocd7c4bdb0241622679d73cecda322","index","CREATE INDEX Ocd7c4bdb0241622679d73cecda322 ON O165bce89be0b4f99d8ddeba7a26a2 (CodecPresetParameter2_)"));
    res.push_back(Database::SchemaItem("O8d965a562afac855c69593367ebb4","index","CREATE INDEX O8d965a562afac855c69593367ebb4 ON O165bce89be0b4f99d8ddeba7a26a2 (CodecPreset1_,CodecPresetParameter2_)"));
    res.push_back(Database::SchemaItem("Oc8e474817e965890acca2b6dd56be","index","CREATE INDEX Oc8e474817e965890acca2b6dd56be ON Ob477e426317c3764439827c70cd95 (CodecPreset1_)"));
    res.push_back(Database::SchemaItem("O747b7bc33250887fdcbd6a51d3026","index","CREATE INDEX O747b7bc33250887fdcbd6a51d3026 ON Ob477e426317c3764439827c70cd95 (Profile2_)"));
    res.push_back(Database::SchemaItem("O0b6de5d92e1109e0ab073510344e8","index","CREATE INDEX O0b6de5d92e1109e0ab073510344e8 ON Ob477e426317c3764439827c70cd95 (CodecPreset1_,Profile2_)"));
    res.push_back(Database::SchemaItem("O64fd619663b0087dc75a48d163c34","index","CREATE INDEX O64fd619663b0087dc75a48d163c34 ON Oc47426250800c92cff81a427efb64 (CodecPreset1_)"));
    res.push_back(Database::SchemaItem("O7941657e8f0fd5f689eb646281984","index","CREATE INDEX O7941657e8f0fd5f689eb646281984 ON Oc47426250800c92cff81a427efb64 (Profile2_)"));
    res.push_back(Database::SchemaItem("Oc27373e559ace4e156f6dee00bcfa","index","CREATE INDEX Oc27373e559ace4e156f6dee00bcfa ON Oc47426250800c92cff81a427efb64 (CodecPreset1_,Profile2_)"));
    res.push_back(Database::SchemaItem("Job_Task_JobTaskJob1_idx","index","CREATE INDEX Job_Task_JobTaskJob1_idx ON Job_Task_JobTask (Job1_)"));
    res.push_back(Database::SchemaItem("Job_Task_JobTaskTask2_idx","index","CREATE INDEX Job_Task_JobTaskTask2_idx ON Job_Task_JobTask (Task2_)"));
    res.push_back(Database::SchemaItem("Job_Task_JobTask_all_idx","index","CREATE INDEX Job_Task_JobTask_all_idx ON Job_Task_JobTask (Job1_,Task2_)"));
    res.push_back(Database::SchemaItem("Graph_Task_GraphTaskGraph1_idx","index","CREATE INDEX Graph_Task_GraphTaskGraph1_idx ON Graph_Task_GraphTask (Graph1_)"));
    res.push_back(Database::SchemaItem("Graph_Task_GraphTaskTask2_idx","index","CREATE INDEX Graph_Task_GraphTaskTask2_idx ON Graph_Task_GraphTask (Task2_)"));
    res.push_back(Database::SchemaItem("Graph_Task_GraphTask_all_idx","index","CREATE INDEX Graph_Task_GraphTask_all_idx ON Graph_Task_GraphTask (Graph1_,Task2_)"));
    res.push_back(Database::SchemaItem("Job_JobLog_JobJobLogJob1_idx","index","CREATE INDEX Job_JobLog_JobJobLogJob1_idx ON Job_JobLog_JobJobLog (Job1_)"));
    res.push_back(Database::SchemaItem("Oc9e847a88b3a0b0d5e4753601554a","index","CREATE INDEX Oc9e847a88b3a0b0d5e4753601554a ON Job_JobLog_JobJobLog (JobLog2_)"));
    res.push_back(Database::SchemaItem("Job_JobLog_JobJobLog_all_idx","index","CREATE INDEX Job_JobLog_JobJobLog_all_idx ON Job_JobLog_JobJobLog (Job1_,JobLog2_)"));
    res.push_back(Database::SchemaItem("Oae2570e4eb6dd44168da7566e665c","index","CREATE INDEX Oae2570e4eb6dd44168da7566e665c ON Job_MediaFile_JobInFile (Job1_)"));
    res.push_back(Database::SchemaItem("Oe85eee8608a5beef1a555fa755f3f","index","CREATE INDEX Oe85eee8608a5beef1a555fa755f3f ON Job_MediaFile_JobInFile (MediaFile2_)"));
    res.push_back(Database::SchemaItem("Odf67e1ff5a62ac6fa96f1213e8b82","index","CREATE INDEX Odf67e1ff5a62ac6fa96f1213e8b82 ON Job_MediaFile_JobInFile (Job1_,MediaFile2_)"));
    res.push_back(Database::SchemaItem("Occb41d354e20ce72ed765acae37e0","index","CREATE INDEX Occb41d354e20ce72ed765acae37e0 ON Job_MediaFile_JobOutFile (Job1_)"));
    res.push_back(Database::SchemaItem("O7139d754cc76b3fee40c054be6406","index","CREATE INDEX O7139d754cc76b3fee40c054be6406 ON Job_MediaFile_JobOutFile (MediaFile2_)"));
    res.push_back(Database::SchemaItem("Od9d2d6c9fa1fb063c88ce1c8efda4","index","CREATE INDEX Od9d2d6c9fa1fb063c88ce1c8efda4 ON Job_MediaFile_JobOutFile (Job1_,MediaFile2_)"));
    res.push_back(Database::SchemaItem("Job_Preset_Job1_idx","index","CREATE INDEX Job_Preset_Job1_idx ON Job_Preset_ (Job1_)"));
    res.push_back(Database::SchemaItem("Job_Preset_Preset2_idx","index","CREATE INDEX Job_Preset_Preset2_idx ON Job_Preset_ (Preset2_)"));
    res.push_back(Database::SchemaItem("Job_Preset__all_idx","index","CREATE INDEX Job_Preset__all_idx ON Job_Preset_ (Job1_,Preset2_)"));
    res.push_back(Database::SchemaItem("Oc3465791d883177f5f8e1eca6fe8e","index","CREATE INDEX Oc3465791d883177f5f8e1eca6fe8e ON Job_JobDetail_JobJobDetail (Job1_)"));
    res.push_back(Database::SchemaItem("O60a494939a5a9e676760baf8b3f68","index","CREATE INDEX O60a494939a5a9e676760baf8b3f68 ON Job_JobDetail_JobJobDetail (JobDetail2_)"));
    res.push_back(Database::SchemaItem("Occe4f09c9234ab0eb75047f7dd450","index","CREATE INDEX Occe4f09c9234ab0eb75047f7dd450 ON Job_JobDetail_JobJobDetail (Job1_,JobDetail2_)"));
    res.push_back(Database::SchemaItem("Oafebf3f48f2ac1cb49f5bd7f5a5b9","index","CREATE INDEX Oafebf3f48f2ac1cb49f5bd7f5a5b9 ON JobDetail_Stream_JobOutStream (JobDetail1_)"));
    res.push_back(Database::SchemaItem("Oc645e43ea9ea28e55a87bbb3cfe85","index","CREATE INDEX Oc645e43ea9ea28e55a87bbb3cfe85 ON JobDetail_Stream_JobOutStream (Stream2_)"));
    res.push_back(Database::SchemaItem("O379b3c10ff7858af2b7df3132c35b","index","CREATE INDEX O379b3c10ff7858af2b7df3132c35b ON JobDetail_Stream_JobOutStream (JobDetail1_,Stream2_)"));
    res.push_back(Database::SchemaItem("O0c5aec68307641ff8c598c19c11d1","index","CREATE INDEX O0c5aec68307641ff8c598c19c11d1 ON JobDetail_Stream_JobInStream (JobDetail1_)"));
    res.push_back(Database::SchemaItem("O56427caf92c562c07f2b954de966b","index","CREATE INDEX O56427caf92c562c07f2b954de966b ON JobDetail_Stream_JobInStream (Stream2_)"));
    res.push_back(Database::SchemaItem("O55609d0353fe020d7d52815110be1","index","CREATE INDEX O55609d0353fe020d7d52815110be1 ON JobDetail_Stream_JobInStream (JobDetail1_,Stream2_)"));
    res.push_back(Database::SchemaItem("Ofea04ea61288d59bff12282f521b2","index","CREATE INDEX Ofea04ea61288d59bff12282f521b2 ON Job_ProcessUnit_ProcessUnitJob (Job1_)"));
    res.push_back(Database::SchemaItem("Oced1d255b23b48639e40e6285b9f1","index","CREATE INDEX Oced1d255b23b48639e40e6285b9f1 ON Job_ProcessUnit_ProcessUnitJob (ProcessUnit2_)"));
    res.push_back(Database::SchemaItem("O0a3fce10cdd119bf165601c22bf63","index","CREATE INDEX O0a3fce10cdd119bf165601c22bf63 ON Job_ProcessUnit_ProcessUnitJob (Job1_,ProcessUnit2_)"));
    res.push_back(Database::SchemaItem("Oce96f93da35b06cfb92be3b1dc1b9","index","CREATE INDEX Oce96f93da35b06cfb92be3b1dc1b9 ON User_UserGroup_User2UserGroup (User1_)"));
    res.push_back(Database::SchemaItem("Oa72b47e3f1ed23207a43635c7dba2","index","CREATE INDEX Oa72b47e3f1ed23207a43635c7dba2 ON User_UserGroup_User2UserGroup (UserGroup2_)"));
    res.push_back(Database::SchemaItem("O5db0c50747293a6fc42d73635c3ad","index","CREATE INDEX O5db0c50747293a6fc42d73635c3ad ON User_UserGroup_User2UserGroup (User1_,UserGroup2_)"));
    res.push_back(Database::SchemaItem("O36936b54601ae855bb8e94cf833bb","index","CREATE INDEX O36936b54601ae855bb8e94cf833bb ON Job_Partition_Job2Partition (Job1_)"));
    res.push_back(Database::SchemaItem("O1d40df53340df5cda1dbdf9272451","index","CREATE INDEX O1d40df53340df5cda1dbdf9272451 ON Job_Partition_Job2Partition (Partition2_)"));
    res.push_back(Database::SchemaItem("O4347eaf0398ab15250636ca7669f7","index","CREATE INDEX O4347eaf0398ab15250636ca7669f7 ON Job_Partition_Job2Partition (Job1_,Partition2_)"));
    res.push_back(Database::SchemaItem("Job_Queue_Job2QueueJob1_idx","index","CREATE INDEX Job_Queue_Job2QueueJob1_idx ON Job_Queue_Job2Queue (Job1_)"));
    res.push_back(Database::SchemaItem("Job_Queue_Job2QueueQueue2_idx","index","CREATE INDEX Job_Queue_Job2QueueQueue2_idx ON Job_Queue_Job2Queue (Queue2_)"));
    res.push_back(Database::SchemaItem("Job_Queue_Job2Queue_all_idx","index","CREATE INDEX Job_Queue_Job2Queue_all_idx ON Job_Queue_Job2Queue (Job1_,Queue2_)"));
    return res;
}
void HiveDb::initialize() {
    static bool initialized = false;
    if (initialized)
        return;
    initialized = true;
    OutputFile::initValues();
    Job::initValues();
    Task::initValues();
    ProcessUnit::initValues();
    Queue::initValues();
}
}
