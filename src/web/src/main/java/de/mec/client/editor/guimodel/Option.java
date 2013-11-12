/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.editor.guimodel;

/**
 *
 * @author jan.hoelscher
 */
public interface Option {
    String getId();
    String getType();
    String getTitle();
    String getGroup();
    String getUnit();
    Control getControl();
    
}
