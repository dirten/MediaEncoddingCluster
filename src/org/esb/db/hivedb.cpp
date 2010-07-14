#include "hivedb.hpp"
namespace db {
using namespace litesql;
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
void MediaFile::defaults() {
    id = 0;
    filesize = 0.0;
    streamcount = 0;
    duration = 0.0;
    bitrate = 0;
    created = 0;
    filetype = 0;
    parent = 0;
}
MediaFile::MediaFile(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), filename(Filename), path(Path), filesize(Filesize), streamcount(Streamcount), containertype(Containertype), duration(Duration), bitrate(Bitrate), created(Created), filetype(Filetype), parent(Parent) {
    defaults();
}
MediaFile::MediaFile(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), filename(Filename), path(Path), filesize(Filesize), streamcount(Streamcount), containertype(Containertype), duration(Duration), bitrate(Bitrate), created(Created), filetype(Filetype), parent(Parent) {
    defaults();
    size_t size = (rec.size() > 12) ? 12 : rec.size();
    switch(size) {
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
     : litesql::Persistent(obj), id(obj.id), type(obj.type), filename(obj.filename), path(obj.path), filesize(obj.filesize), streamcount(obj.streamcount), containertype(obj.containertype), duration(obj.duration), bitrate(obj.bitrate), created(obj.created), filetype(obj.filetype), parent(obj.parent) {
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
}
void MediaFile::delRecord() {
    deleteFromTable(table__, id);
}
void MediaFile::delRelations() {
    MediaFileProjectRelation::del(*db, (MediaFileProjectRelation::MediaFile == id));
    MediaFileStreamRelation::del(*db, (MediaFileStreamRelation::MediaFile == id));
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
const litesql::FieldType Profile::Vframerate("vframerate_","INTEGER",table__);
const litesql::FieldType Profile::Vwidth("vwidth_","INTEGER",table__);
const litesql::FieldType Profile::Vheight("vheight_","INTEGER",table__);
const litesql::FieldType Profile::Vextra("vextra_","TEXT",table__);
const litesql::FieldType Profile::Acodec("acodec_","INTEGER",table__);
const litesql::FieldType Profile::Achannels("achannels_","INTEGER",table__);
const litesql::FieldType Profile::Abitrate("abitrate_","INTEGER",table__);
const litesql::FieldType Profile::Asamplerate("asamplerate_","INTEGER",table__);
const litesql::FieldType Profile::Aextra("aextra_","TEXT",table__);
const litesql::FieldType Profile::Profiletype("profiletype_","INTEGER",table__);
void Profile::defaults() {
    id = 0;
    created = 0;
    vcodec = 0;
    vbitrate = 0;
    vframerate = 0;
    vwidth = 0;
    vheight = 0;
    acodec = 0;
    achannels = 0;
    abitrate = 0;
    asamplerate = 0;
    profiletype = 0;
}
Profile::Profile(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name), created(Created), format(Format), formatext(Formatext), vcodec(Vcodec), vbitrate(Vbitrate), vframerate(Vframerate), vwidth(Vwidth), vheight(Vheight), vextra(Vextra), acodec(Acodec), achannels(Achannels), abitrate(Abitrate), asamplerate(Asamplerate), aextra(Aextra), profiletype(Profiletype) {
    defaults();
}
Profile::Profile(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name), created(Created), format(Format), formatext(Formatext), vcodec(Vcodec), vbitrate(Vbitrate), vframerate(Vframerate), vwidth(Vwidth), vheight(Vheight), vextra(Vextra), acodec(Acodec), achannels(Achannels), abitrate(Abitrate), asamplerate(Asamplerate), aextra(Aextra), profiletype(Profiletype) {
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
    case 14: achannels = convert<const std::string&, int>(rec[13]);
        achannels.setModified(false);
    case 13: acodec = convert<const std::string&, int>(rec[12]);
        acodec.setModified(false);
    case 12: vextra = convert<const std::string&, std::string>(rec[11]);
        vextra.setModified(false);
    case 11: vheight = convert<const std::string&, int>(rec[10]);
        vheight.setModified(false);
    case 10: vwidth = convert<const std::string&, int>(rec[9]);
        vwidth.setModified(false);
    case 9: vframerate = convert<const std::string&, int>(rec[8]);
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
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name), created(obj.created), format(obj.format), formatext(obj.formatext), vcodec(obj.vcodec), vbitrate(obj.vbitrate), vframerate(obj.vframerate), vwidth(obj.vwidth), vheight(obj.vheight), vextra(obj.vextra), acodec(obj.acodec), achannels(obj.achannels), abitrate(obj.abitrate), asamplerate(obj.asamplerate), aextra(obj.aextra), profiletype(obj.profiletype) {
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
        acodec = obj.acodec;
        achannels = obj.achannels;
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
    fields.push_back(acodec.name());
    values.push_back(acodec);
    acodec.setModified(false);
    fields.push_back(achannels.name());
    values.push_back(achannels);
    achannels.setModified(false);
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
    updateField(updates, table__, acodec);
    updateField(updates, table__, achannels);
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
    ftypes.push_back(Acodec);
    ftypes.push_back(Achannels);
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
    np->acodec = acodec;
    np->achannels = achannels;
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
    os << o.acodec.name() << " = " << o.acodec << std::endl;
    os << o.achannels.name() << " = " << o.achannels << std::endl;
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
HiveDb::HiveDb(std::string backendType, std::string connInfo)
     : litesql::Database(backendType, connInfo) {
    initialize();
}
std::vector<litesql::Database::SchemaItem> HiveDb::getSchema() const {
    vector<Database::SchemaItem> res;
    res.push_back(Database::SchemaItem("schema_","table","CREATE TABLE schema_ (name_ TEXT, type_ TEXT, sql_ TEXT);"));
    if (backend->supportsSequences()) {
        res.push_back(Database::SchemaItem("Project_seq","sequence","CREATE SEQUENCE Project_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("MediaFile_seq","sequence","CREATE SEQUENCE MediaFile_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("Profile_seq","sequence","CREATE SEQUENCE Profile_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("Stream_seq","sequence","CREATE SEQUENCE Stream_seq START 1 INCREMENT 1"));
    }
    res.push_back(Database::SchemaItem("Project_","table","CREATE TABLE Project_ (id_ " + backend->getRowIDType() + ",type_ TEXT,name_ TEXT,created_ INTEGER)"));
    res.push_back(Database::SchemaItem("MediaFile_","table","CREATE TABLE MediaFile_ (id_ " + backend->getRowIDType() + ",type_ TEXT,filename_ TEXT,path_ TEXT,filesize_ DOUBLE,streamcount_ INTEGER,containertype_ TEXT,duration_ DOUBLE,bitrate_ INTEGER,created_ INTEGER,filetype_ INTEGER,parent_ INTEGER)"));
    res.push_back(Database::SchemaItem("Profile_","table","CREATE TABLE Profile_ (id_ " + backend->getRowIDType() + ",type_ TEXT,name_ TEXT,created_ INTEGER,format_ TEXT,formatext_ TEXT,vcodec_ INTEGER,vbitrate_ INTEGER,vframerate_ INTEGER,vwidth_ INTEGER,vheight_ INTEGER,vextra_ TEXT,acodec_ INTEGER,achannels_ INTEGER,abitrate_ INTEGER,asamplerate_ INTEGER,aextra_ TEXT,profiletype_ INTEGER)"));
    res.push_back(Database::SchemaItem("Stream_","table","CREATE TABLE Stream_ (id_ " + backend->getRowIDType() + ",type_ TEXT,streamindex_ INTEGER,streamtype_ INTEGER,codecid_ INTEGER,codecname_ TEXT,frameratenum_ INTEGER,framerateden_ INTEGER,streamtimebasenum_ INTEGER,streamtimebaseden_ INTEGER,codectimebasenum_ INTEGER,codectimebaseden_ INTEGER,firstpts_ DOUBLE,firstdts_ DOUBLE,duration_ DOUBLE,nbframes_ DOUBLE,ticksperframe_ INTEGER,framecount_ INTEGER,width_ INTEGER,height_ INTEGER,gopsize_ INTEGER,pixfmt_ INTEGER,bitrate_ INTEGER,samplerate_ INTEGER,samplefmt_ INTEGER,channels_ INTEGER,bitspercodedsample_ INTEGER,privdatasize_ INTEGER,privdata_ TEXT,extradatasize_ INTEGER,extradata_ TEXT,flags_ INTEGER,extraprofileflags_ TEXT)"));
    res.push_back(Database::SchemaItem("MediaFile_Project_","table","CREATE TABLE MediaFile_Project_ (MediaFile1 INTEGER UNIQUE,Project2 INTEGER)"));
    res.push_back(Database::SchemaItem("Profile_Project_","table","CREATE TABLE Profile_Project_ (Profile1 INTEGER UNIQUE,Project2 INTEGER)"));
    res.push_back(Database::SchemaItem("MediaFile_Stream_","table","CREATE TABLE MediaFile_Stream_ (MediaFile1 INTEGER,Stream2 INTEGER UNIQUE)"));
    res.push_back(Database::SchemaItem("MediaFile_Project_MediaFile1idx","index","CREATE INDEX MediaFile_Project_MediaFile1idx ON MediaFile_Project_ (MediaFile1)"));
    res.push_back(Database::SchemaItem("MediaFile_Project_Project2idx","index","CREATE INDEX MediaFile_Project_Project2idx ON MediaFile_Project_ (Project2)"));
    res.push_back(Database::SchemaItem("MediaFile_Project__all_idx","index","CREATE INDEX MediaFile_Project__all_idx ON MediaFile_Project_ (MediaFile1,Project2)"));
    res.push_back(Database::SchemaItem("Profile_Project_Profile1idx","index","CREATE INDEX Profile_Project_Profile1idx ON Profile_Project_ (Profile1)"));
    res.push_back(Database::SchemaItem("Profile_Project_Project2idx","index","CREATE INDEX Profile_Project_Project2idx ON Profile_Project_ (Project2)"));
    res.push_back(Database::SchemaItem("Profile_Project__all_idx","index","CREATE INDEX Profile_Project__all_idx ON Profile_Project_ (Profile1,Project2)"));
    res.push_back(Database::SchemaItem("MediaFile_Stream_MediaFile1idx","index","CREATE INDEX MediaFile_Stream_MediaFile1idx ON MediaFile_Stream_ (MediaFile1)"));
    res.push_back(Database::SchemaItem("MediaFile_Stream_Stream2idx","index","CREATE INDEX MediaFile_Stream_Stream2idx ON MediaFile_Stream_ (Stream2)"));
    res.push_back(Database::SchemaItem("MediaFile_Stream__all_idx","index","CREATE INDEX MediaFile_Stream__all_idx ON MediaFile_Stream_ (MediaFile1,Stream2)"));
    return res;
}
void HiveDb::initialize() {
    static bool initialized = false;
    if (initialized)
        return;
    initialized = true;
}
}
