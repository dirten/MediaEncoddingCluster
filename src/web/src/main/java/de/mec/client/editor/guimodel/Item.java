/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.editor.guimodel;

import java.util.List;
import java.util.Map;

/**
 *
 * @author jan.hoelscher
 */
public interface Item {
    String getKey();
    String getValue();
    Map<String,String>getData();
    List<String> getEnables();
}
