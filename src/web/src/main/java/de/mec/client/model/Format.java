/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.model;

import com.google.web.bindery.autobean.shared.AutoBean;

/**
 *
 * @author jhoelscher
 */
public interface Format {

  String getId();

  void setId(String id);

  @AutoBean.PropertyName(value = "longname")
  String getLongName();

  @AutoBean.PropertyName(value = "longname")
  void setLongName(String name);
}
