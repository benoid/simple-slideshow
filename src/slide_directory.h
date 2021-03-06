/*  Copyright (C) 2016 David Benoit

    This file is part of SimpleSlideshow.

    SimpleSlideshow is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SimpleSlideshow is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SimpleSlideshow.  If not, see <http://www.gnu.org/licenses/>. */

/* slide_directory.h

   Author: David Benoit
   Purpose: Container class representing
            a directory of slides.  Stores
            information about the name, file path,
            whether the directory contains informational/
            marketing slides, and whether the user has chosen
            to exclude the directory from the slideshow. */

#ifndef SLIDEDIRECTORY_H
#define SLIDEDIRECTORY_H

#include <QString>
#include <QStringList>

class SlideDirectory
{


public:
  SlideDirectory();
/*** Accessors ***/
  QString name();
  QString full_path();
  QStringList file_list();
  bool is_marketing_folder();
  bool is_excluded();

/*** Mutators ***/
  void set_dir_name(QString set_name);
  void set_full_path(QString set_path);
  void append_file_list(QString file);
  void set_marketing_folder(bool set_marketing_bool);
  void set_exclude_from_show(bool set_exclude_bool);
private:
  QString name_;
  QString full_path_;
  QStringList file_list_;
  bool marketing_folder_;
  bool exclude_from_slideshow_;

};

#endif // SLIDEDIRECTORY_H
