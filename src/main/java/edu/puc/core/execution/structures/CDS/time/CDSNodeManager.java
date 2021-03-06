package edu.puc.core.execution.structures.CDS.time;

import edu.puc.core.parser.plan.cea.Transition;
import edu.puc.core.runtime.events.Event;

import java.lang.ref.WeakReference;


public class CDSNodeManager {

    private final LinkedNodeList NodeList;

    public CDSNodeManager() {
        NodeList = new LinkedNodeList();
    }

    public CDSTimeNode createOutputNode(CDSTimeNode child, Transition.TransitionType transitionType, Event event, long currentTime) {
        CDSTimeOutputNode newNode = new CDSTimeOutputNode(child, transitionType, event, currentTime);
        NodeList.add(new LinkedNode(newNode));
        return newNode;
    }

    public CDSTimeUnionNode createUnionNode(CDSTimeNode left, CDSTimeNode right) {
        CDSTimeUnionNode newNode;
        if (left.getMm() > right.getMm()) {
            newNode = new CDSTimeUnionNode(new WeakReference<>(left), new WeakReference<>(right));
        } else if (right.getMm() > left.getMm()) {
            newNode = new CDSTimeUnionNode(new WeakReference<>(right), new WeakReference<>(left));
        } else if (left instanceof CDSTimeOutputNode) {
            newNode = new CDSTimeUnionNode(new WeakReference<>(left), new WeakReference<>(right));
        } else {
            if (right instanceof CDSTimeOutputNode) {
                newNode = new CDSTimeUnionNode(new WeakReference<>(right), new WeakReference<>(left));
            } else {
                CDSTimeUnionNode tempLeft = (CDSTimeUnionNode) left;
                CDSTimeUnionNode tempRight = (CDSTimeUnionNode) right;

                WeakReference<CDSTimeNode> leftRight = tempLeft.getRightReference();
                WeakReference<CDSTimeNode> rightRight = tempRight.getRightReference();

                CDSTimeNode tempLeftRight = leftRight.get();
                CDSTimeNode tempRightRight = rightRight.get();
                CDSTimeUnionNode u2;

                if (tempLeftRight == null) {
                     u2 = new CDSTimeUnionNode(rightRight, leftRight);
                } else if (tempRightRight == null) {
                    u2 = new CDSTimeUnionNode(leftRight, rightRight);
                } else if (tempLeftRight.getMm() > tempRightRight.getMm()) {
                    u2 = new CDSTimeUnionNode(leftRight, rightRight);
                } else {
                    u2 = new CDSTimeUnionNode(rightRight, leftRight);
                }


                NodeList.add(new LinkedNode(u2));
                CDSTimeUnionNode u1 = new CDSTimeUnionNode(tempRight.getLeftReference(), new WeakReference<>(u2));
                NodeList.add(new LinkedNode(u1));
                newNode = new CDSTimeUnionNode(tempLeft.getLeftReference(), new WeakReference<>(u1));
            }
        }
        NodeList.add(new LinkedNode(newNode));
        return newNode;
    }

    public CDSTimeBottomNode createBottomNode(long currentTime) {
        CDSTimeBottomNode newNode = new CDSTimeBottomNode(currentTime);
        NodeList.add(new LinkedNode(newNode));

        return newNode;
    }

    public void prune(long timestamp, long limit) {
        LinkedNode newFirst = null;
        for (LinkedNode ln: NodeList) {
            if (timestamp - ln.getData().mm < limit) {
                newFirst = ln;
                break;
            }
        }
        NodeList.setHead(newFirst);
    }
}
