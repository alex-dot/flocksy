/*
 *
 *
 */

#ifndef F_DIRS_HPP
#define F_DIRS_HPP
#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <boost/filesystem.hpp>
#include <vector>
#include <unordered_map>
#include <string>

#include "constants.hpp"
#include "hash.hpp"
#include "hash_tree.hpp"

class Directory
{
  public: 
    Directory();
    Directory(const boost::filesystem::path&);
    ~Directory();

    void fillDirectory(const boost::filesystem::path&, 
                       std::vector<boost::filesystem::directory_entry>&);

    HashTree* getHashTree() const;

    bool checkDirectoryChange(const Directory& left) const;
    bool getChangedEntryHashes(std::vector< std::shared_ptr<Hash> >& changed_hashes,
                               const Directory& left) const;

    const std::string           getPath() const;
    const std::string           getAbsolutePath() const;
          int                   getNumberOfEntries() const;
    const std::shared_ptr<Hash> getDirectoryHash() const;

    void setSymlinkHandling(symlink_handling_t);

  private:
    void makeDirectoryHash();
    void processDirectoryEntry(const boost::filesystem::directory_entry& entry,
                               std::vector< std::shared_ptr<Hash> >& temp_hashes,
                               std::vector<boost::filesystem::directory_entry>& dirs);
    void processRegularFileEntry(const boost::filesystem::directory_entry& entry,
                                 std::vector< std::shared_ptr<Hash> >& temp_hashes);

    boost::filesystem::path path_;
    std::unordered_map<std::string,boost::filesystem::directory_entry> entries_;
    HashTree* hash_tree_;
    std::shared_ptr<Hash> directory_hash_;
    symlink_handling_t symlinks_;
};

#endif