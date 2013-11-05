/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.model;

import com.google.web.bindery.autobean.shared.AutoBean.PropertyName;

/**
 *
 * @author jan.hoelscher
 */
public interface Audio {

  String getId();

  void setId(String id);

  @PropertyName(value = "ar")
  Integer getSampleRate();

  @PropertyName(value = "ar")
  void setSampleRate(Integer samplerate);

  @PropertyName(value = "ab")
  Integer getBitrate();

  @PropertyName(value = "ab")
  void setBitrate(Integer rate);

  @PropertyName(value = "ac")
  Integer getChannels();

  @PropertyName(value = "ac")
  void setChannels(Integer channels);
  /*
   */
}
