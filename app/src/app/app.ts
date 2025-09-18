import { Component, OnInit } from '@angular/core';
import { PrimeNG } from 'primeng/config';
import { palette } from '@primeuix/themes';

@Component({
    selector: 'app-root',
    templateUrl: './app.html',
    standalone: false,
    styleUrl: './app.scss'
})
export class App implements OnInit {
    protected title = 'app';

    constructor(private primeng: PrimeNG) {}


    ngOnInit(): void {
        this.primeng.ripple.set(true);
    }
}
