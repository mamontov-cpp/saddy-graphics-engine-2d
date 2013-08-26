/** \file qspritetablewidgetselection.h
	\author HiddenSeeker

	Implements a selection for table widget
 */
#include <QString>
#pragma once

/** A class of selection for QSpriteTableWidget
 */
class QSpriteTableWidgetSelection
{
 private:
	/** A config to work with
	 */
	QString m_config;
	/** A group to work with
	 */
	QString m_group;
	/** Index for data
	 */
	int m_index;
 public:
	/** A sprite widget selection
		\param[in] config config data
		\param[in] group group data
		\param[in] index index data
	 */
	QSpriteTableWidgetSelection(const QString & config, const QString & group, int index);
	/** Returns a config
		\return config to work with
	 */ 
	inline const QString & config() const { return m_config; }
	/** Group of sprites 
		\return group of sprite
	 */
	inline const QString & group() const { return m_group; }
	/** Index of sprites
		\return index in group of sprites
	 */
	inline int  index() const { return m_index; }
    /** Checks whether selection is invalid
		\return, whether it's invalid
	 */
	bool invalid() const;
};