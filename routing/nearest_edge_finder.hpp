#pragma once

#include "routing/road_graph.hpp"
#include "routing/vehicle_model.hpp"

#include "geometry/point2d.hpp"

#include "indexer/feature_decl.hpp"
#include "indexer/index.hpp"
#include "indexer/mwm_set.hpp"

#include "std/unique_ptr.hpp"
#include "std/utility.hpp"
#include "std/vector.hpp"

namespace routing
{

/// Helper functor class to filter nearest roads to the given starting point.
/// Class returns pairs of outgoing edge and projection point on the edge
class NearestEdgeFinder
{
  struct Candidate
  {
    double m_dist;
    uint32_t m_segId;
    m2::PointD m_segStart;
    m2::PointD m_segEnd;
    m2::PointD m_point;
    FeatureID m_fid;

    Candidate();
  };

  m2::PointD const m_point;
  vector<Candidate> m_candidates;

public:
  NearestEdgeFinder(m2::PointD const & point);

  inline bool HasCandidates() const { return !m_candidates.empty(); }

  void AddInformationSource(FeatureID const & featureId, IRoadGraph::RoadInfo const & roadInfo);

  void MakeResult(vector<pair<Edge, m2::PointD>> & res, size_t const maxCountFeatures);
};

}  // namespace routing
