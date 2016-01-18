#include "settings_window_view.h"
#include "ui_settings_window_view.h"
#include <QString>
#include <QDebug>
#include <QDir>
#include <QFileDialog>

void SettingsWindowView::apply_settings()
{
  int temp;

  // Update config settings if valid
  temp =  ui->lineEdit->value();
  if (temp > 0)
    {
      main_timer_interval_ = temp * 1000;
    }
  temp =  ui->lineEdit_2->value();
  if (temp > 0)
    {
      marketing_timer_interval_ = temp * 1000;
    }
  main_sort_order_ = (QueueSortOrder) ui->comboBox->currentIndex();
  marketing_sort_order_ = (QueueSortOrder) ui->comboBox_2->currentIndex();
  marketing_playback_option_ = (MarketingPlaybackOption) ui->checkBox_6->checkState();
  fullscreen_disabled_bool_ = ui->checkBox->isChecked();
  begin_on_marketing_slide_bool_ = ui->checkBox_5->isChecked();

  for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
      bool match_found = false;
      for (int j = 0; !match_found && j < slide_directory_list_.size(); ++j)
        {
          // Check for equal match
          if (ui->tableWidget->item(i,0)->text()
              == slide_directory_list_[j].name())
            {
              match_found = true;
              if (ui->tableWidget->item(i,2)->checkState())
                {
                  QDir(slide_directory_list_[j].full_path()).removeRecursively();
                }
              else if (!ui->tableWidget->item(i,1)->checkState())
                {
                  // File should be hidden
                  QDir().rename(slide_directory_list_[j].full_path(),
                                slide_folder_path_ + "/."
                                + slide_directory_list_[j].name());
                }
            }
          // Check for opposite match
          else if (slide_directory_list_[j].name().at(0) == '.')
            {
              if (slide_directory_list_[j].name().remove(0,1)
                  == ui->tableWidget->item(i,0)->text())
                {
                  match_found = true;
                  if (ui->tableWidget->item(i,2)->checkState())
                    {
                      //QDir(slide_directory_list_[j].full_path()).removeRecursively();
                    }
                  else if (ui->tableWidget->item(i,1)->checkState())
                    {
                      // File should be hidden
                      slide_directory_list_[j].set_dir_name(
                         ui->tableWidget->item(i,0)->text());
                      QDir().rename(slide_directory_list_[j].full_path(),
                                slide_folder_path_ + "/"
                                + slide_directory_list_[j].name());
                    }
                }
            }
        }
    }
}

SettingsWindowView::SettingsWindowView(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SettingsWindowView)
{
  ui->setupUi(this);
  this->setWindowTitle("Application Settings");
  this->setFixedSize(724, 385);
  this->setWindowFlags(Qt::WindowStaysOnTopHint);
  connect(ui->tableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), ui->tableWidget, SLOT(sortByColumn(int)));

}

SettingsWindowView::~SettingsWindowView()
{
  delete ui;
}

void SettingsWindowView::set_up_gui()
{

  // Clear contents of table
  ui->tableWidget->clearContents();
  while (ui->tableWidget->rowCount() > 0)
    {
      ui->tableWidget->removeRow(0);
    }
  // Set up table
  for (int i = 0; i < slide_directory_list_.size(); ++i)
    {
      // Column 1
      table_item = new QTableWidgetItem;
      table_item->setText(slide_directory_list_[i].name());
      table_item->setFlags(table_item->flags() ^ Qt::ItemIsEditable);
      ui->tableWidget->insertRow(0);
      ui->tableWidget->setItem(0, 0, table_item);

      // Column 2
      table_item = new QTableWidgetItem;
      if(ui->tableWidget->item(0,0)->text().at(0) == '.')
        {
          // Strip dot from hidden folder
          ui->tableWidget->item(0,0)->setText(
          ui->tableWidget->item(0,0)->text().remove(0,1));
          table_item->setCheckState(Qt::Unchecked);
        }
      else
        {
          table_item->setCheckState(Qt::Checked);
        }
      ui->tableWidget->setItem(0, 1, table_item);

      // Column 3
      table_item = new QTableWidgetItem;
      table_item->setCheckState(Qt::Unchecked);
      ui->tableWidget->setItem(0, 2, table_item);

    }
  ui->tableWidget->resizeColumnsToContents();

  ui->lineEdit->setValue(main_timer_interval_/1000.0);
  ui->lineEdit_2->setValue(marketing_timer_interval_/1000.0);
  ui->lineEdit_4->setText(slide_folder_path());

  ui->checkBox->setChecked(fullscreen_disabled_bool_);
  ui->checkBox_5->setChecked(begin_on_marketing_slide_bool_);
  ui->comboBox->setCurrentIndex((int)main_sort_order_);
  ui->comboBox_2->setCurrentIndex((int)marketing_sort_order_);
  ui->checkBox_6->setChecked((bool) marketing_playback_option_);
  ui->tableWidget->hideColumn(2);

  // Disable changing folder location
  // ui->lineEdit_4->setDisabled(true);
  // ui->pushButton_2->setDisabled(true);
}

void SettingsWindowView::set_focus_main_tab()
{
  ui->tabWidget->setCurrentIndex(0);
}

void SettingsWindowView::set_focus_folders_tab()
{
  ui->tabWidget->setCurrentIndex(1);
}

void SettingsWindowView::set_slide_directory_list(QList<SlideDirectory> dir_list)
{
  slide_directory_list_ = dir_list;
}

void SettingsWindowView::on_buttonBox_accepted()
{
  apply_settings();
  emit ok_button_pressed();
}

void SettingsWindowView::on_buttonBox_rejected()
{
  emit cancel_button_pressed();
}

// Update but don't close
void SettingsWindowView::on_pushButton_clicked()
{
  apply_settings();
  emit apply_button_pressed();
}

void SettingsWindowView::on_pushButton_2_clicked()
{
  QString temp = QFileDialog::getExistingDirectory(this, tr("Select Remote Image Folder"), QDir::homePath(), QFileDialog::ReadOnly | QFileDialog::ShowDirsOnly);
  if (temp != "")
    {
      ui->lineEdit_4->setText(temp);
    }
}
