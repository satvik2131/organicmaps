#pragma once

#include "defines.hpp"

#include "base/logging.hpp"

#include "coding/file_name_utils.hpp"

#include "std/string.hpp"
#include "std/vector.hpp"

namespace feature
{

struct GenerateInfo
{
  enum class NodeStorageType
  {
    Memory,
    Index,
    File
  };

  enum class OsmSourceType
  {
    XML,
    O5M
  };


  // Directory for .mwm.tmp files.
  string m_tmpDir;
  // Directory for result .mwm files.
  string m_targetDir;
  // Directory for all intermediate files.
  string m_intermediateDir;

  // Current generated file name if --output option is defined.
  string m_fileName;

  NodeStorageType m_nodeStorageType;
  OsmSourceType m_osmFileType;
  string m_osmFileName;

  vector<string> m_bucketNames;

  bool m_createWorld;
  bool m_splitByPolygons;
  bool m_makeCoasts;
  bool m_emitCoasts;
  bool m_genAddresses;
  bool m_failOnCoasts;


  GenerateInfo()
    : m_createWorld(false), m_splitByPolygons(false),
      m_makeCoasts(false), m_emitCoasts(false), m_genAddresses(false)
  {
  }

  void SetOsmFileType(string const & type)
  {
    if (type == "xml")
      m_osmFileType = OsmSourceType::XML;
    else if (type == "o5m")
      m_osmFileType = OsmSourceType::O5M;
    else
      LOG(LCRITICAL, ("Unknown source type:", type));
  }

  void SetNodeStorageType(string const & type)
  {
    if (type == "raw")
      m_nodeStorageType = NodeStorageType::File;
    else if (type == "map")
      m_nodeStorageType = NodeStorageType::Index;
    else if (type == "mem")
      m_nodeStorageType = NodeStorageType::Memory;
    else
      LOG(LCRITICAL, ("Incorrect node_storage type:", type));
  }

  string GetTmpFileName(string const & fileName, char const * ext = DATA_FILE_EXTENSION_TMP) const
  {
    return my::JoinFoldersToPath(m_tmpDir, fileName + ext);
  }
  string GetTargetFileName(string const & fileName, char const * ext = DATA_FILE_EXTENSION) const
  {
    return my::JoinFoldersToPath(m_targetDir, fileName + ext);
  }
  string GetIntermediateFileName(string const & fileName, char const * ext = DATA_FILE_EXTENSION) const
  {
    return my::JoinFoldersToPath(m_intermediateDir, fileName + ext);
  }
  string GetAddressesFileName() const
  {
    return ((m_genAddresses && !m_fileName.empty()) ? GetTargetFileName(m_fileName, ADDR_FILE_EXTENSION) : string());
  }
};

} // namespace feature
