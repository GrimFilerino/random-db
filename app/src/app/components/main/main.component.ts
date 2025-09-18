import { Component, OnDestroy, OnInit, ViewChild } from '@angular/core';
import { Drawer } from 'primeng/drawer';

@Component({
    selector: 'main',
    standalone: false,
    templateUrl: './main.component.html',
    styleUrl: './main.component.scss'
})
export class Main implements OnInit, OnDestroy {
    collections = [
        {
            key: '0',
            label: 'Documents',
            data: 'Documents Folder',
            icon: 'pi pi-fw pi-inbox',
            children: [
                {
                    key: '0-0',
                    label: 'Work',
                    data: 'Work Folder',
                    icon: 'pi pi-fw pi-cog',
                    children: [
                        { key: '0-0-0', label: 'Expenses.doc', icon: 'pi pi-fw pi-file', data: 'Expenses Document' },
                        { key: '0-0-1', label: 'Resume.doc', icon: 'pi pi-fw pi-file', data: 'Resume Document' }
                    ]
                },
                {
                    key: '0-1',
                    label: 'Home',
                    data: 'Home Folder',
                    icon: 'pi pi-fw pi-home',
                    children: [{ key: '0-1-0', label: 'Invoices.txt', icon: 'pi pi-fw pi-file', data: 'Invoices for this month' }]
                }
            ]
        },
    ];
    visible: boolean = false;

    @ViewChild('drawerRef') drawerRef!: Drawer;
    constructor() {
    }

    closeCallback(e:any): void {
        this.drawerRef.close(e);
    }

    ngOnInit() {
    }

    ngOnDestroy() {
    }
}
