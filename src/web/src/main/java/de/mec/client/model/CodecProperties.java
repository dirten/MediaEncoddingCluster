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
 * @author jan.hoelscher
 */
public interface CodecProperties extends PropertyAccess<Codec> {

        ModelKeyProvider<Codec> id();

        LabelProvider<Codec> longName();
    }

