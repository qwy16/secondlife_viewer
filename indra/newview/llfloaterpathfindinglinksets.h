/** 
 * @file llfloaterpathfindinglinksets.h
 * @author William Todd Stinson
 * @brief "Pathfinding linksets" floater, allowing manipulation of the Havok AI pathfinding settings.
 *
 * $LicenseInfo:firstyear=2002&license=viewerlgpl$
 * Second Life Viewer Source Code
 * Copyright (C) 2010, Linden Research, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation;
 * version 2.1 of the License only.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * Linden Research, Inc., 945 Battery Street, San Francisco, CA  94111  USA
 * $/LicenseInfo$
 */

#ifndef LL_LLFLOATERPATHFINDINGLINKSETS_H
#define LL_LLFLOATERPATHFINDINGLINKSETS_H

#include "llsd.h"
#include "v3math.h"
#include "llfloater.h"
#include "lluuid.h"

class LLTextBase;
class LLScrollListCtrl;

class PathfindingLinkset
{
public:
	PathfindingLinkset(const std::string &pUUID, const LLSD &pNavmeshItem);
	PathfindingLinkset(const PathfindingLinkset& pOther);
	virtual ~PathfindingLinkset();

	PathfindingLinkset& operator = (const PathfindingLinkset& pOther);

	const LLUUID&      getUUID() const;
	const std::string& getName() const;
	const std::string& getDescription() const;
	U32                getLandImpact() const;
	const LLVector3&   getPositionAgent() const;

	BOOL               isFixed() const;
	void               setFixed(BOOL pIsFixed);

	BOOL               isWalkable() const;
	void               setWalkable(BOOL pIsWalkable);

	BOOL               isPhantom() const;
	void               setPhantom(BOOL pIsPhantom);

	F32                getA() const;
	void               setA(F32 pA);

	F32                getB() const;
	void               setB(F32 pB);

	F32                getC() const;
	void               setC(F32 pC);

	F32                getD() const;
	void               setD(F32 pD);

protected:

private:
	LLUUID      mUUID;
	std::string mName;
	std::string mDescription;
	U32         mLandImpact;
	LLVector3   mLocation;
	BOOL        mIsFixed;
	BOOL        mIsWalkable;
	BOOL        mIsPhantom;
	F32         mA;
	F32         mB;
	F32         mC;
	F32         mD;
};

class PathfindingLinksets
{
public:
	typedef std::map<std::string, PathfindingLinkset> PathfindingLinksetMap;

	PathfindingLinksets();
	PathfindingLinksets(const LLSD& pNavmeshData);
	PathfindingLinksets(const PathfindingLinksets& pOther);
	virtual ~PathfindingLinksets();

	void parseNavmeshData(const LLSD& pNavmeshData);
	void clearLinksets();

	const PathfindingLinksetMap& getAllLinksets() const;
	const PathfindingLinksetMap& getFilteredLinksets();

	BOOL isFilterActive() const;
	void setNameFilter(const std::string& pNameFilter);
	void clearFilters();

protected:

private:
	PathfindingLinksetMap mAllLinksets;
	PathfindingLinksetMap mFilteredLinksets;

	bool        mIsFilterDirty;
	std::string mNameFilter;

	void applyFilters();
};

class LLFloaterPathfindingLinksets
:	public LLFloater
{
	friend class LLFloaterReg;
	friend class NavmeshDataGetResponder;

public:
	typedef enum
	{
		kFetchInitial,
		kFetchStarting,
		kFetchInProgress,
		kFetchInProgress_MultiRequested,
		kFetchReceived,
		kFetchError,
		kFetchComplete
	} EFetchState;

	virtual BOOL postBuild();
	virtual void onOpen(const LLSD& pKey);

	static void openLinksetsEditor();

	EFetchState getFetchState() const;
	BOOL        isFetchInProgress() const;

protected:

private:
	PathfindingLinksets mPathfindingLinksets;
	EFetchState         mFetchState;
	LLScrollListCtrl    *mLinksetsScrollList;
	LLTextBase          *mLinksetsStatus;

	// Does its own instance management, so clients not allowed
	// to allocate or destroy.
	LLFloaterPathfindingLinksets(const LLSD& pSeed);
	virtual ~LLFloaterPathfindingLinksets();

	void sendNavmeshDataGetRequest();
	void handleNavmeshDataGetReply(const LLSD& pNavmeshData);
	void handleNavmeshDataGetError(const std::string& pURL, const std::string& pErrorReason);

	void setFetchState(EFetchState pFetchState);

	void onLinksetsSelectionChange();
	void onRefreshLinksetsClicked();
	void onSelectAllLinksetsClicked();
	void onSelectNoneLinksetsClicked();

	void clearLinksetsList();
	void selectAllLinksets();
	void selectNoneLinksets();

	void updateLinksetsStatusMessage();
};

#endif // LL_LLFLOATERPATHFINDINGLINKSETS_H