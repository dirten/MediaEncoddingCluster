/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.model;

import com.sencha.gxt.data.shared.LabelProvider;
import com.sencha.gxt.data.shared.ModelKeyProvider;
import com.sencha.gxt.data.shared.PropertyAccess;

/**
 *
 * @author jhoelscher
 */
public interface FormatProperties extends PropertyAccess<Format> {

  ModelKeyProvider<Format> id();

  LabelProvider<Format> longName();
}
