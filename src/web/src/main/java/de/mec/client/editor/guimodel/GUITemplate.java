/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.editor.guimodel;

import java.util.List;

/**
 *
 * @author jan.hoelscher
 */
public interface GUITemplate {

    List<Group> getGroups();
    List<Option> getOptions();
}
