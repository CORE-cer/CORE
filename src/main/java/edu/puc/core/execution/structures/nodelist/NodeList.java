package edu.puc.core.execution.structures.nodelist;

import edu.puc.core.execution.structures.CDS.time.CDSNodeManager;
import edu.puc.core.execution.structures.CDS.time.CDSTimeNode;
import edu.puc.core.execution.structures.CDS.time.CDSTimeUnionNode;

import java.util.ArrayList;

public class NodeList {

    private final ArrayList<CDSTimeNode> nodeList;
    private final CDSNodeManager manager;

    public NodeList(CDSNodeManager manager) {
        nodeList = new ArrayList<>();
        this.manager = manager;
    }

    public CDSTimeNode merge() {
        if (nodeList.isEmpty()) {
            return null;
        } else if (nodeList.size() == 1) {
            return nodeList.get(0);
        } else if (nodeList.size() == 2) {
            return manager.createUnionNode(nodeList.get(1), nodeList.get(0));
        } else {
            CDSTimeUnionNode last = manager.createUnionNode(nodeList.get(1), nodeList.get(0));
            for (int i = 2; i < nodeList.size(); i++) {
                last = manager.createUnionNode(nodeList.get(i), last);
            }
            return last;
        }
    }

    public void addSorted(CDSTimeNode newNode) {
        if (nodeList.isEmpty()) {
            nodeList.add(newNode);
        } else {
            for (int i = nodeList.size() - 2; i >= 0; i--) {
                CDSTimeNode current = nodeList.get(i);
                if (newNode.getMm() == current.getMm()) {
                    nodeList.set(i, manager.createUnionNode(newNode, current));
                    return;
                } else if (newNode.getMm() > current.getMm()) {
                    nodeList.add(i, newNode);
                    return;
                }
            }
            nodeList.add(0, newNode);
        }
    }

    public boolean isEmpty() {
        return nodeList.isEmpty();
    }

    public CDSTimeNode getHead() {
        if (nodeList.isEmpty()) {
            return null;
        }
        return nodeList.get(0);
    }

}
