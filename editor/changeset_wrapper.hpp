#pragma once

#include "base/exception.hpp"

#include "editor/server_api.hpp"
#include "editor/xml_feature.hpp"

#include "geometry/point2d.hpp"
#include "geometry/rect2d.hpp"

#include "std/set.hpp"
#include "std/vector.hpp"

class FeatureType;

namespace osm
{
struct ClientToken;

class ChangesetWrapper
{
  using TTypeCount = map<string, uint16_t>;

public:
  DECLARE_EXCEPTION(ChangesetWrapperException, RootException);
  DECLARE_EXCEPTION(NetworkErrorException, ChangesetWrapperException);
  DECLARE_EXCEPTION(HttpErrorException, ChangesetWrapperException);
  DECLARE_EXCEPTION(OsmXmlParseException, ChangesetWrapperException);
  DECLARE_EXCEPTION(OsmObjectWasDeletedException, ChangesetWrapperException);
  DECLARE_EXCEPTION(CreateChangeSetFailedException, ChangesetWrapperException);
  DECLARE_EXCEPTION(ModifyNodeFailedException, ChangesetWrapperException);
  DECLARE_EXCEPTION(LinearFeaturesAreNotSupportedException, ChangesetWrapperException);
  // TODO: Remove this when relations are handled properly.
  DECLARE_EXCEPTION(RelationFeatureAreNotSupportedException, ChangesetWrapperException);
  DECLARE_EXCEPTION(EmptyFeatureException, ChangesetWrapperException);

  ChangesetWrapper(TKeySecret const & keySecret,
                   ServerApi06::TKeyValueTags const & comments) noexcept;
  ~ChangesetWrapper();

  /// Throws many exceptions from above list, plus including XMLNode's parsing ones.
  /// OsmObjectWasDeletedException means that node was deleted from OSM server by someone else.
  editor::XMLFeature GetMatchingNodeFeatureFromOSM(m2::PointD const & center);
  editor::XMLFeature GetMatchingAreaFeatureFromOSM(vector<m2::PointD> const & geomerty);

  /// Throws exceptions from above list.
  void Create(editor::XMLFeature node);

  /// Throws exceptions from above list.
  /// Node should have correct OSM "id" attribute set.
  void Modify(editor::XMLFeature node);

  /// Throws exceptions from above list.
  void Delete(editor::XMLFeature node);

private:
  /// Unfortunately, pugi can't return xml_documents from methods.
  /// Throws exceptions from above list.
  void LoadXmlFromOSM(ms::LatLon const & ll, pugi::xml_document & doc);
  void LoadXmlFromOSM(m2::RectD const & rect, pugi::xml_document & doc);

  ServerApi06::TKeyValueTags m_changesetComments;
  ServerApi06 m_api;
  static constexpr uint64_t kInvalidChangesetId = 0;
  uint64_t m_changesetId = kInvalidChangesetId;

  // No m_deleted_types here, since we do not delete features.
  TTypeCount m_modified_types;
  TTypeCount m_created_types;
  string TypeCountToString(TTypeCount const & typeCount) const;
  string GetDescription() const;
};

}  // namespace osm
