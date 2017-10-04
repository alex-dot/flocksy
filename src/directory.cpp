/*
 * Directory
 *
 */

#include "directory.hpp"
#include "hash.hpp"
#include "hash_tree.hpp"

#include <boost/filesystem.hpp>
#include <vector>
#include <unordered_map>
#include <utility>
#include <memory>

#include <stdio.h>
#include <string>
#include <cmath>

Directory::Directory() :
  path_(),
  entries_(),
  hash_tree_(),
  directory_hash_(std::shared_ptr<Hash>(new Hash())),
  symlinks_(F_SYMLINK_DEFAULT)
  {}

Directory::Directory(const boost::filesystem::path& p) :
  path_(p),
  entries_(),
  hash_tree_(),
  directory_hash_(std::shared_ptr<Hash>(new Hash())),
  symlinks_(F_SYMLINK_DEFAULT)
  {
    std::vector<boost::filesystem::directory_entry> dirs;
    fillDirectory(path_, dirs);
  }

Directory::~Directory()
{
  //delete hashes
  delete hash_tree_;
}

void Directory::fillDirectory(const boost::filesystem::path& document_root, 
                              std::vector<boost::filesystem::directory_entry>& dirs)
{
  // first define document_root the directory's root path
  path_ = document_root;

  // TODO: need EXCEPTION handling for empty hash/document root
  std::vector< std::shared_ptr<Hash> > temp_hashes;

  // iterate over the given path and write every file to entries_, return directories
  for ( boost::filesystem::directory_iterator i = boost::filesystem::directory_iterator(path_); 
        i != boost::filesystem::directory_iterator(); 
        ++i )
  {
    try
    {
      this->processDirectoryEntry(*i, temp_hashes, dirs);
    }
    catch (const std::runtime_error& err)
    {
      printf("You have an error in your filesystem! \n");
      throw;
    }
  }
  std::cout << temp_hashes.size() << std::endl;
  HashTree* temp_ht = new HashTree();
  temp_ht->makeHashTree(temp_hashes);
  std::swap(hash_tree_,temp_ht);
  delete temp_ht;
  this->makeDirectoryHash();
}

void Directory::makeDirectoryHash()
{
  std::string hash_string = hash_tree_->getTopHash()->getString();
  hash_string += this->getPath();
  directory_hash_->makeHash(hash_string);
}
void Directory::processDirectoryEntry(const boost::filesystem::directory_entry& entry,
                                      std::vector< std::shared_ptr<Hash> >& temp_hashes,
                                      std::vector<boost::filesystem::directory_entry>& dirs)
{
  if (entry.symlink_status().type() == 4 && this->symlinks_ == F_SYMLINK_FOLLOW)
  {
    boost::filesystem::directory_entry sl(read_symlink(entry));
    this->processDirectoryEntry(sl, temp_hashes, dirs);
  }

  else if (entry.symlink_status().type() == 4 && this->symlinks_ == F_SYMLINK_COPY)
    processRegularFileEntry(entry, temp_hashes);

  else if (is_directory(entry) && entry.symlink_status().type() != 4)
    dirs.push_back(entry);

  else if (is_regular_file(entry) && entry.symlink_status().type() != 4)
    processRegularFileEntry(entry, temp_hashes);

  else
    if (F_MSG_DEBUG) printf("dir: special file ignored\n");
}
void Directory::processRegularFileEntry(const boost::filesystem::directory_entry& entry,
                                        std::vector< std::shared_ptr<Hash> >& temp_hashes)
{
  std::string string_to_hash = "";

  // add filename to string
  boost::filesystem::path file = entry.path();
  std::string filename = file.filename().c_str();
  string_to_hash += filename;

  // add file path relative to path_ to string
  int document_root_length = strlen(path_.c_str());
  std::string relative_file_path = file.c_str();
  relative_file_path = relative_file_path.substr(document_root_length,
                                                   (strlen(file.c_str())-filename.length())-document_root_length);
  string_to_hash += relative_file_path;

  // add timestamp to string
  string_to_hash += std::to_string(boost::filesystem::last_write_time(file));

  // make hash
  std::shared_ptr<Hash> hash(new Hash(string_to_hash));
  temp_hashes.push_back(hash);

  // insert into entries_
  entries_.insert(std::make_pair(hash->getString(),entry));
}

HashTree* Directory::getHashTree() const { return hash_tree_; }
bool Directory::checkDirectoryChange(const Directory& left) const
{
  return ( *left.getDirectoryHash() == *this->getDirectoryHash() ) ? false : true;
}
bool Directory::getChangedEntryHashes(std::vector< std::shared_ptr<Hash> >& changed_hashes,
                       const Directory& left) const
{
  return hash_tree_->getChangedHashes(changed_hashes, *(left.getHashTree()));
}

const std::string Directory::getPath() const { return path_.filename().c_str(); }
const std::string Directory::getAbsolutePath() const { return path_.c_str(); }
      int         Directory::getNumberOfEntries() const { return entries_.size(); }
const std::shared_ptr<Hash> Directory::getDirectoryHash()  const { return directory_hash_; }

void Directory::setSymlinkHandling(symlink_handling_t symlink_handling) { this->symlinks_ = symlink_handling; }
